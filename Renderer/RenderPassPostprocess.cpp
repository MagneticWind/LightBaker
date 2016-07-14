#include <assert.h>

#include "CBufferDefs.h"
#include "GPUResourceManager.h"

#include "HALgfx\IBuffer.h"
#include "HALgfx\IDevice.h"
#include "HALgfx\IDeviceContext.h"
#include "HALgfx\IRenderTargetView.h"
#include "HALgfx\ISamplerState.h"
#include "HALgfx\IShaderResourceView.h"
#include "HALgfx\ITexture2d.h"
#include "HALgfx\IUnorderedAccessView.h"

#include "Math\Math.h"

#include "RenderPassPostprocess.h"
#include "ShaderNode.h"

namespace Magnet
{
namespace Renderer
{

namespace RenderPassPostprocessPrivate
{
	static const float TONEMAPPING_TEXEL_SIZE = 81.f;
	static const float TONEMAPPING_TEXEL_SCALE = 1.f / (TONEMAPPING_TEXEL_SIZE * TONEMAPPING_TEXEL_SIZE);
	static const float REDUCTION_BLOCK_SIZE = 8.f;
	static const float REDUCTION_1D_BLOCK_SIZE = 128.f;
}

//------------------------------------------------------------------
RenderPassPostprocess::RenderPassPostprocess() : m_pSSAOShaderNode(NULL), m_pTonemappingShaderNode(NULL),
		m_bInitialized(false), 
		m_pQuadVertexBuffer(NULL), m_pQuadIndexBuffer(NULL), m_pInputLayout(NULL),
		m_pSRVHDR(NULL), m_pSRVDepth(NULL),
		m_pSSAOTexture(NULL), m_pSSAOTextureSRV(NULL), m_pSSAORTV(NULL),
		m_pReductionBuffer0(NULL), m_pReductionBuffer1(NULL),
		m_pReductionSRV0(NULL), m_pReductionSRV1(NULL),
		m_pReductionUAV0(NULL), m_pReductionUAV1(NULL)

{
}

//------------------------------------------------------------------
RenderPassPostprocess::~RenderPassPostprocess()
{
	if (m_pSSAOShaderNode)
	{
		delete m_pSSAOShaderNode;
		m_pSSAOShaderNode = NULL;
	}

	if (m_pTonemappingShaderNode)
	{
		delete m_pTonemappingShaderNode;
		m_pTonemappingShaderNode = NULL;
	}

	if (m_pQuadVertexBuffer)
	{
		delete m_pQuadVertexBuffer;
		m_pQuadVertexBuffer = NULL;
	}

	if (m_pQuadIndexBuffer)
	{
		delete m_pQuadIndexBuffer;
		m_pQuadIndexBuffer = NULL;
	}

	if (m_pInputLayout)
	{
		delete m_pInputLayout;
		m_pInputLayout = NULL;
	}

	if (m_pSRVHDR)
	{
		delete m_pSRVHDR;
		m_pSRVHDR = NULL;
	}

	if (m_pSRVDepth)
	{
		delete m_pSRVDepth;
		m_pSRVDepth = NULL;
	}

	if (m_pSSAOTexture)
	{
		delete m_pSSAOTexture;
		m_pSSAOTexture = NULL;
	}

	if (m_pSSAOTextureSRV)
	{
		delete m_pSSAOTextureSRV;
		m_pSSAOTextureSRV = NULL;
	}

	if (m_pSSAORTV)
	{
		delete m_pSSAORTV;
		m_pSSAORTV = NULL;
	}

	if (m_pReduceTo1DComputeShader)
	{
		delete m_pReduceTo1DComputeShader;
		m_pReduceTo1DComputeShader = NULL;
	}

	if (m_pReduceTo1PixelComputeShader)
	{
		delete m_pReduceTo1PixelComputeShader;
		m_pReduceTo1PixelComputeShader = NULL;
	}

	if (m_pReductionBuffer0)
	{
		delete m_pReductionBuffer0;
		m_pReductionBuffer0 = NULL;
	}

	if (m_pReductionBuffer1)
	{
		delete m_pReductionBuffer1;
		m_pReductionBuffer1 = NULL;
	}

	if (m_pReductionSRV0)
	{
		delete m_pReductionSRV0;
		m_pReductionSRV0 = NULL;
	}

	if (m_pReductionSRV1)
	{
		delete m_pReductionSRV1;
		m_pReductionSRV1 = NULL;
	}

	if (m_pReductionUAV0)
	{
		delete m_pReductionUAV0;
		m_pReductionUAV0 = NULL;
	}

	if (m_pReductionUAV1)
	{
		delete m_pReductionUAV1;
		m_pReductionUAV1 = NULL;
	}

}

//------------------------------------------------------------------
void RenderPassPostprocess::SetRenderState(HALgfx::IDeviceContext* pDeviceContext, const HALgfx::ViewPort& viewPort,
	HALgfx::IRenderTargetView* pRTV, HALgfx::IDepthStencilView* pDSV, HALgfx::IRasterizerState* pRState, HALgfx::IDepthStencilState* pDSState)
{
	pDeviceContext->SetViewPort(viewPort);
	pDeviceContext->SetRasterizerState(pRState);
	pDeviceContext->SetDepthStencilState(pDSState);

	//pDeviceContext->ClearRenderTargetView(pRTV, Math::Vector4f(0, 0, 0, 0));
	//pDeviceContext->ClearDepthStencilView(pDSV, HALgfx::CLEAR_DEPTH, 1.f, 0);

	pDeviceContext->SetPrimitiveTopology(HALgfx::PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//pDeviceContext->SetRenderTargetViews(1, &pRTV, pDSV);
	m_pFinalRTV = pRTV;
	m_pDSV = pDSV;

}

//------------------------------------------------------------------
template <class T>
static void SWAP(T* &x, T* &y)
{
	T* temp = x;
	x = y;
	y = temp;
}

//------------------------------------------------------------------
void RenderPassPostprocess::Render(HALgfx::IDevice* pDevice, HALgfx::IDeviceContext* pDeviceContext)
{
	pDeviceContext->BeginEvent("RenderPassPostprocess");

	// ssao
	{
		pDeviceContext->SetRenderTargetViews(1, &m_pSSAORTV, NULL);
		pDeviceContext->ClearRenderTargetView(m_pSSAORTV, Math::Vector4f(0, 0, 0, 0));
		//pDeviceContext->ClearDepthStencilView(m_pDSV, HALgfx::CLEAR_DEPTH, 1.f, 0);
		m_pSSAOShaderNode->Draw(pDeviceContext);
	}

	// compute avg luminance
	{

		pDeviceContext->BeginEvent("avg luminance");

		int dimx = int(ceil(RenderPassPostprocessPrivate::TONEMAPPING_TEXEL_SIZE / RenderPassPostprocessPrivate::REDUCTION_BLOCK_SIZE));
		int dimy = dimx;

		CBufferReduceTo1D cbCS;
		cbCS.dimX = dimx;
		cbCS.dimY = dimy;
		cbCS.textureX = m_iDimensionX;
		cbCS.textureY = m_iDimensionY;

		pDeviceContext->BeginEvent("reduceto1d");
		m_pReduceTo1DComputeShader->RunCompute(pDeviceContext, 1, &m_pSRVHDR,
			&cbCS, 1, &m_pReductionUAV0,
			dimx, dimy, 1);
		pDeviceContext->EndEvent();

		// pingpong to reduce to 1 pixel
		pDeviceContext->BeginEvent("reduceto1p");

		int dim = dimx*dimy;
		int nNumToReduce = dim;
		dim = int(ceil(dim / RenderPassPostprocessPrivate::REDUCTION_1D_BLOCK_SIZE));
		if (nNumToReduce > 1)
		{
			char debugStr[64];
			for (;;)
			{
				CBufferReduceTo1Pixel cbCS;
				cbCS.numToReduce = nNumToReduce;
				cbCS.dim = dim;
				cbCS.placeHolder1 = 0;
				cbCS.placeHolder2 = 0;

				sprintf(debugStr, "reduce_%d", nNumToReduce);
				pDeviceContext->BeginEvent(debugStr);
				m_pReduceTo1PixelComputeShader->RunCompute(pDeviceContext, 1, &m_pReductionSRV0,
					&cbCS, 1, &m_pReductionUAV1,
					dim, 1, 1);
				pDeviceContext->EndEvent();

				nNumToReduce = dim;
				dim = int(ceil(dim / RenderPassPostprocessPrivate::REDUCTION_1D_BLOCK_SIZE));

				if (nNumToReduce == 1)
					break;
				
				SWAP(m_pReductionBuffer0, m_pReductionBuffer1);
				SWAP(m_pReductionSRV0, m_pReductionSRV1);
				SWAP(m_pReductionUAV0, m_pReductionUAV1);
			}
		}
		else
		{
			SWAP(m_pReductionBuffer0, m_pReductionBuffer1);
			SWAP(m_pReductionSRV0, m_pReductionSRV1);
			SWAP(m_pReductionUAV0, m_pReductionUAV1);
		}
		pDeviceContext->EndEvent();

		pDeviceContext->EndEvent();
	}

	// tone mapping
	{

		DrawNode& drawNode = m_pTonemappingShaderNode->GetDrawNode("quad");

		CBufferToneMapping* pBuffer = static_cast<CBufferToneMapping*>(drawNode.m_pPSCBufferData[0]);
		pBuffer->v4Param.x = m_iDimensionX;
		pBuffer->v4Param.y = m_iDimensionY;
		pBuffer->v4Param.z = RenderPassPostprocessPrivate::TONEMAPPING_TEXEL_SCALE;

		drawNode.AddSRV(m_pReductionSRV1);

		pDeviceContext->SetRenderTargetViews(1, &m_pFinalRTV, NULL);
		pDeviceContext->ClearRenderTargetView(m_pFinalRTV, Math::Vector4f(0, 0, 0, 0));
		//	pDeviceContext->ClearDepthStencilView(m_pDSV, HALgfx::CLEAR_DEPTH, 1.f, 0);
		m_pTonemappingShaderNode->Draw(pDeviceContext);

		drawNode.RemoveSRV(m_pReductionSRV1);
	}

	pDeviceContext->EndEvent();
}

//------------------------------------------------------------------
PassType RenderPassPostprocess::GetType()
{
	return PASS_POSTPROCESS;
}

//------------------------------------------------------------------
void RenderPassPostprocess::ClearDrawNodes()
{
	m_pSSAOShaderNode->ClearDrawNodes();
	m_pTonemappingShaderNode->ClearDrawNodes();
}

//------------------------------------------------------------------
void RenderPassPostprocess::Setup(HALgfx::IDevice* pDevice, int iWidth, int iHeight)
{
	if (!m_bInitialized)
	{
		Initialize(pDevice, iWidth, iHeight);
		m_bInitialized = true;
	}

}

//------------------------------------------------------------------
void RenderPassPostprocess::Initialize(HALgfx::IDevice* pDevice, int iWidth, int iHeight)
{
	float vertexData[5 * 4] = {
		-1.f, 1.f, 0.f, 0.f, 0.f,
		-1.f, -1.f, 0.f, 0.f, 1.f,
		1.f, -1.f, 0.f, 1.f, 1.f,
		1.f, 1.f, 0.f, 1.f, 0.f
	};

	int indexData[3 * 2] = {
		0, 2, 1,
		0, 3, 2
	};

	HALgfx::InputElementDesc aInputElementsDesc[2];
	strcpy(aInputElementsDesc[0].semantic,"POSITION");
	strcpy(aInputElementsDesc[1].semantic, "TEXCOORD");
	aInputElementsDesc[0].format = HALgfx::FORMAT_R32G32B32_FLOAT;
	aInputElementsDesc[0].inputSlot = 0;
	aInputElementsDesc[0].alignedByteOffset = 0;
	aInputElementsDesc[0].semanticIndex = 0;
	aInputElementsDesc[0].instanceDataStepRate = 0;
	aInputElementsDesc[0].inputClassification = HALgfx::INPUT_PER_VERTEX_DATA;

	aInputElementsDesc[1].format = HALgfx::FORMAT_R32G32_FLOAT;
	aInputElementsDesc[1].inputSlot = 0;
	aInputElementsDesc[1].alignedByteOffset = 12;
	aInputElementsDesc[1].semanticIndex = 0;
	aInputElementsDesc[1].instanceDataStepRate = 0;
	aInputElementsDesc[1].inputClassification = HALgfx::INPUT_PER_VERTEX_DATA;

	// vertex buffer
	HALgfx::BufferDesc desc;
	desc.bindFlags = HALgfx::BIND_VERTEX_BUFFER;
	desc.byteStride = 20; // 1 verter has 5 floats
	desc.byteSize = desc.byteStride * 4; // 4 vertices
	//desc.cpuAccessFlags = HALgfx::CPU_ACCESS_READ;
	desc.usage = HALgfx::USAGE_DEFAULT;
	//desc.miscFlags
	HALgfx::SubResourceData data;
	data.pMem = static_cast<void*>(vertexData);
	m_pQuadVertexBuffer = pDevice->CreateBuffer(desc, data);

	// index buffer
	desc.bindFlags = HALgfx::BIND_INDEX_BUFFER;
	desc.byteStride = sizeof(int) * 3; // 3 ints
	desc.byteSize = desc.byteStride * 2; // 2 triangles
	//desc.cpuAccessFlags = HALgfx::CPU_ACCESS_READ;
	desc.usage = HALgfx::USAGE_DEFAULT;
	data.pMem = static_cast<void*>(indexData);
	m_pQuadIndexBuffer = pDevice->CreateBuffer(desc, data);

	// view and projection matrix from an orthonormal projection
	Math::Vector3f v3Eye(0.f, 0.f, -1.f);
	Math::Vector3f v3Up(0.f, 1.f, 0.f);
	Math::Vector3f v3Lookat(0.f, 0.f, 0.f);
	Math::Matrix4f mWorld, mView, mProjection;
	Math::MatrixLookAtLH(v3Eye, v3Lookat, v3Up, mView);
	Math::MatrixOrthographicLH(2.f, 2.f, 0.0f, 1000.f, mProjection);
	//Math::MatrixPerspectiveFovLH(60.f / 180.f * 3.14f, 1024.f/768.f, 0.01f, 1000.f, pTransform->mProjection);

	// ssao shader
	{
		// resources
		HALgfx::Texture2dDesc texDesc;
		texDesc.format = HALgfx::FORMAT_R8G8B8A8_UNORM;
		texDesc.width = iWidth;
		texDesc.height = iHeight;
		texDesc.mipLevels = 1;
		texDesc.arraySize = 1;
		texDesc.bindFlags = HALgfx::BIND_RENDER_TARGET | HALgfx::BIND_SHADER_RESOURCE;
		texDesc.cpuAccessFlags = 0;
		texDesc.sampleDesc.uCount = 1;
		texDesc.usage = HALgfx::USAGE_DEFAULT;
		HALgfx::SubResourceData subResourceData;
		subResourceData.pMem = 0;
		m_pSSAOTexture = pDevice->CreateTexture2d(texDesc, &subResourceData);

		HALgfx::RenderTargetViewDesc rtvDesc;
		rtvDesc.format = texDesc.format;
		rtvDesc.viewDimension = HALgfx::RTV_DIMENSION_TEXTURE2D;
		rtvDesc.texture2d.mipSlice = 0;
		m_pSSAORTV = pDevice->CreateRenderTargetView(m_pSSAOTexture, rtvDesc);

		HALgfx::ShaderResourceViewDesc srvDesc;
		srvDesc.format = texDesc.format;
		srvDesc.viewDimension = HALgfx::SRV_DIMENSION_TEXTURE2D;
		srvDesc.texSRV.mipLevels = 1;
		srvDesc.texSRV.mostDetailedMip = 0;
		m_pSSAOTextureSRV = pDevice->CreateShaderResourceView(m_pSSAOTexture, srvDesc);

		// SSAO samples
		GenerateSSAOSamples();

		char shaderName[256];
		strcpy(shaderName, "ssao");
		m_pSSAOShaderNode = new ShaderNode(shaderName, pDevice);

		m_pSSAOShaderNode->LoadShader(HALgfx::VERTEX_SHADER);
		m_pSSAOShaderNode->LoadShader(HALgfx::PIXEL_SHADER);

		// create const buffers
		HALgfx::BufferDesc desc;
		HALgfx::SubResourceData data;
		desc.bindFlags = HALgfx::BIND_CONSTANT_BUFFER;
		desc.byteSize = sizeof(CBufferTransform);
		desc.cpuAccessFlags = HALgfx::CPU_ACCESS_WRITE;
		desc.usage = HALgfx::USAGE_DYNAMIC;
		m_pSSAOShaderNode->AddConstantBuffer(desc, pDevice, HALgfx::VERTEX_SHADER);

		desc.byteSize = sizeof(CBufferSSAO);
		m_pSSAOShaderNode->AddConstantBuffer(desc, pDevice, HALgfx::PIXEL_SHADER);

		// input layout
		m_pSSAOShaderNode->CreateInputLayout(2, aInputElementsDesc, 20,  pDevice);

		// texture label
		m_pSSAOShaderNode->AddTextureLabel(Scene::TEXTURE_LABEL_DEPTH);

		m_pSSAOShaderNode->Create(2, aInputElementsDesc, pDevice);

		GPUResourceManager& gpuResourceManager = GPUResourceManager::GetInstance();

		// draw node
		DrawNode drawNodeSSAO;

		strcpy(drawNodeSSAO.m_caName, "quad");

		// vertex buffer and index buffer
		drawNodeSSAO.m_pVertexBuffer = m_pQuadVertexBuffer;
		drawNodeSSAO.m_pIndexBuffer = m_pQuadIndexBuffer;
		drawNodeSSAO.m_iNumberOfPrimitives = 2;

		// const buffers
		CBufferTransform* pTransform = static_cast<CBufferTransform*>(drawNodeSSAO.CreateCBufferData(sizeof(CBufferTransform), HALgfx::VERTEX_SHADER));
		pTransform->mWorld = mWorld;
		pTransform->mView = mView;
		pTransform->mProjection = mProjection;

		CBufferSSAO* pSSAO = static_cast<CBufferSSAO*>(drawNodeSSAO.CreateCBufferData(sizeof(CBufferSSAO), HALgfx::PIXEL_SHADER));
		*pSSAO = m_ssaoParams;

		// depth texture
		drawNodeSSAO.AddSRV(m_pSRVDepth);
		HALgfx::ISamplerState* pSampler = gpuResourceManager.GetSamplerState(Scene::SAMPLER_NOMIP_LINEAR_WRAP, pDevice);
		drawNodeSSAO.AddSampler(pSampler);

		m_pSSAOShaderNode->AddDrawNode(drawNodeSSAO, pDevice);
	}

	// avg luminance
	{
		char shaderName[256];
		strcpy(shaderName, "reduceto1d");
		m_pReduceTo1DComputeShader = new ShaderNode(shaderName, pDevice);
		m_pReduceTo1DComputeShader->LoadShader(HALgfx::COMPUTE_SHADER);
		m_pReduceTo1DComputeShader->Create(2, aInputElementsDesc, pDevice);

		strcpy(shaderName, "reduceto1pixel");
		m_pReduceTo1PixelComputeShader = new ShaderNode(shaderName, pDevice);
		m_pReduceTo1PixelComputeShader->LoadShader(HALgfx::COMPUTE_SHADER);
		m_pReduceTo1PixelComputeShader->Create(2, aInputElementsDesc, pDevice);

		// create const buffers
		HALgfx::BufferDesc desc;
		HALgfx::SubResourceData data;
		desc.bindFlags = HALgfx::BIND_CONSTANT_BUFFER;
		desc.byteSize = sizeof(CBufferReduceTo1D);
		desc.cpuAccessFlags = HALgfx::CPU_ACCESS_WRITE;
		desc.usage = HALgfx::USAGE_DYNAMIC;
		m_pReduceTo1DComputeShader->AddConstantBuffer(desc, pDevice, HALgfx::COMPUTE_SHADER);

		desc.byteSize = sizeof(CBufferReduceTo1Pixel);
		m_pReduceTo1PixelComputeShader->AddConstantBuffer(desc, pDevice, HALgfx::COMPUTE_SHADER);

		// pingpong buffers, uavs, srvs
		HALgfx::BufferDesc descBuffer;
		HALgfx::SubResourceData bufferData;
		bufferData.pMem = NULL;
		descBuffer.bindFlags = HALgfx::BIND_UNORDERED_ACCESS | HALgfx::BIND_SHADER_RESOURCE;
		descBuffer.byteSize = int(ceil(m_iDimensionX / RenderPassPostprocessPrivate::REDUCTION_BLOCK_SIZE) * ceil(m_iDimensionY / RenderPassPostprocessPrivate::REDUCTION_BLOCK_SIZE)) * sizeof(float);
		descBuffer.miscFlags = HALgfx::MISC_BUFFER_STRUCTURED;
		descBuffer.byteStride = sizeof(float);
		descBuffer.usage = HALgfx::USAGE_DEFAULT;
		m_pReductionBuffer0 = pDevice->CreateBuffer(descBuffer, bufferData);
		m_pReductionBuffer1 = pDevice->CreateBuffer(descBuffer, bufferData);

		HALgfx::UnorderedAccessViewDesc descUAV;
		descUAV.format = HALgfx::FORMAT_UNKNOWN;
		descUAV.viewDimension = HALgfx::UAV_DIMENSION_BUFFER;
		descUAV.bufferUAV.firstElement = 0;
		descUAV.bufferUAV.numElements = descBuffer.byteSize / sizeof(float);
		descUAV.bufferUAV.flags = 0;
		m_pReductionUAV0 = pDevice->CreateUnorderedAccessView(m_pReductionBuffer0, descUAV);
		m_pReductionUAV1 = pDevice->CreateUnorderedAccessView(m_pReductionBuffer1, descUAV);

		HALgfx::ShaderResourceViewDesc descSRV;
		descSRV.format = HALgfx::FORMAT_UNKNOWN;
		descSRV.viewDimension = HALgfx::SRV_DIMENSION_BUFFER;
		descSRV.bufferSRV.firstElement = descUAV.bufferUAV.firstElement;
		descSRV.bufferSRV.numElements = descUAV.bufferUAV.numElements;
		m_pReductionSRV0 = pDevice->CreateShaderResourceView(m_pReductionBuffer0, descSRV);
		m_pReductionSRV1 = pDevice->CreateShaderResourceView(m_pReductionBuffer1, descSRV);

	}

	// tone mapping shader
	{
		char shaderName[256];
		strcpy(shaderName, "tonemapping");
		m_pTonemappingShaderNode = new ShaderNode(shaderName, pDevice);

		m_pTonemappingShaderNode->LoadShader(HALgfx::VERTEX_SHADER);
		m_pTonemappingShaderNode->LoadShader(HALgfx::PIXEL_SHADER);

		// create const buffers
		HALgfx::BufferDesc desc;
		HALgfx::SubResourceData data;
		desc.bindFlags = HALgfx::BIND_CONSTANT_BUFFER;
		desc.byteSize = sizeof(CBufferTransform);
		desc.cpuAccessFlags = HALgfx::CPU_ACCESS_WRITE;
		desc.usage = HALgfx::USAGE_DYNAMIC;
		m_pTonemappingShaderNode->AddConstantBuffer(desc, pDevice, HALgfx::VERTEX_SHADER);

		desc.byteSize = sizeof(CBufferToneMapping);
		m_pTonemappingShaderNode->AddConstantBuffer(desc, pDevice, HALgfx::PIXEL_SHADER);

		// input layout
		m_pTonemappingShaderNode->CreateInputLayout(2, aInputElementsDesc, 20, pDevice);

		// texture label
		m_pTonemappingShaderNode->AddTextureLabel(Scene::TEXTURE_LABEL_FRAME);

		m_pTonemappingShaderNode->Create(2, aInputElementsDesc, pDevice);

		GPUResourceManager& gpuResourceManager = GPUResourceManager::GetInstance();

		// draw node
		DrawNode drawNodeTonemapping;

		strcpy(drawNodeTonemapping.m_caName, "quad");

		// vertex buffer and index buffer
		drawNodeTonemapping.m_pVertexBuffer = m_pQuadVertexBuffer;
		drawNodeTonemapping.m_pIndexBuffer = m_pQuadIndexBuffer;
		drawNodeTonemapping.m_iNumberOfPrimitives = 2;

		// const buffers
		CBufferTransform* pTransform = static_cast<CBufferTransform*>(drawNodeTonemapping.CreateCBufferData(sizeof(CBufferTransform), HALgfx::VERTEX_SHADER));
		pTransform->mWorld = mWorld;
		pTransform->mView = mView;
		pTransform->mProjection = mProjection;

		CBufferToneMapping* pTonemap = static_cast<CBufferToneMapping*>(drawNodeTonemapping.CreateCBufferData(sizeof(CBufferToneMapping), HALgfx::PIXEL_SHADER));
		int iDimensionX, iDimensionY;

		pTonemap->v4Param.x = static_cast<float>(1024);
		pTonemap->v4Param.y = static_cast<float>(1024);
		pTonemap->v4Param.z = 10.f;

		// framebuffer texture
		drawNodeTonemapping.AddSRV(m_pSRVHDR);

		HALgfx::ISamplerState* pSampler = gpuResourceManager.GetSamplerState(Scene::SAMPLER_NOMIP_LINEAR_WRAP, pDevice);
		drawNodeTonemapping.AddSampler(pSampler);

		m_pTonemappingShaderNode->AddDrawNode(drawNodeTonemapping, pDevice);
	}

}

//------------------------------------------------------------------
void RenderPassPostprocess::GenerateSSAOSamples()
{
	m_ssaoParams.f4SamplePoints[0] = Math::Vector4f(0.2f, 0.1f, 0.f, 0.f);
	m_ssaoParams.f4SamplePoints[1] = Math::Vector4f(0.5f, 0.08f, 0.f, 0.f);
	m_ssaoParams.f4SamplePoints[2] = Math::Vector4f(0.1f, 0.7f, 0.f, 0.f);
	m_ssaoParams.f4SamplePoints[3] = Math::Vector4f(-0.15f, 0.45f, 0.f, 0.f);
	m_ssaoParams.f4SamplePoints[4] = Math::Vector4f(-0.65f, 0.5f, 0.f, 0.f);

	float fTotalLength = 0.f;
	for (int i = 0; i < NUM_SAMPLER_PAIRS; ++i)
	{
		Math::Vector4f& sample = m_ssaoParams.f4SamplePoints[i];
		sample.z = sqrtf(1.0 - sample.x * sample.x - sample.y * sample.y);
		fTotalLength += sample.z;
	}
	// compute percentage
	fTotalLength = fTotalLength * 2 + 1.f;
	for (int i = 0; i < NUM_SAMPLER_PAIRS; ++i)
	{
		Math::Vector4f& sample = m_ssaoParams.f4SamplePoints[i];
		sample.w = sample.z / float(NUM_SAMPLER_PAIRS * 2 + 1);
	}

	m_ssaoParams.fCenterWeight = 1.f / fTotalLength;
}

void RenderPassPostprocess::ComputeAvgLuminance()
{

}

} // namespace Renderer
} // namespace Magnet