#include <assert.h>

#include "CBufferDefs.h"
#include "GPUResourceManager.h"

#include "HALgfx\IBuffer.h"
#include "HALgfx\IDevice.h"
#include "HALgfx\IDeviceContext.h"
#include "HALgfx\ISamplerState.h"

#include "Math\Math.h"

#include "RenderPassPostprocess.h"
#include "ShaderNode.h"

namespace Magnet
{
namespace Renderer
{
//------------------------------------------------------------------
RenderPassPostprocess::RenderPassPostprocess() : m_bInitialized(false), 
		m_pQuadVertexBuffer(NULL), m_pQuadIndexBuffer(NULL), m_pInputLayout(NULL)
{
}

//------------------------------------------------------------------
RenderPassPostprocess::~RenderPassPostprocess()
{
	std::list<ShaderNode*>::iterator it = m_lShaderNodes.begin();
	std::list<ShaderNode*>::iterator itEnd = m_lShaderNodes.end();
	while (it != itEnd)
	{
		ShaderNode* pNode = *it;
		delete pNode;
		pNode = 0;
		it++;
	}
	m_lShaderNodes.clear();

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
}

//------------------------------------------------------------------
void RenderPassPostprocess::SetRenderState(HALgfx::IDeviceContext* pDeviceContext, const HALgfx::ViewPort& viewPort,
	HALgfx::IRenderTargetView* pRTV, HALgfx::IDepthStencilView* pDSV, HALgfx::IRasterizerState* pRState, HALgfx::IDepthStencilState* pDSState)
{
	pDeviceContext->SetViewPort(viewPort);
	pDeviceContext->SetRenderTargetViews(1, &pRTV, pDSV);
	pDeviceContext->SetRasterizerState(pRState);
	pDeviceContext->SetDepthStencilState(pDSState);

	pDeviceContext->ClearRenderTargetView(pRTV, Math::Vector4f(0, 0, 0, 0));
	pDeviceContext->ClearDepthStencilView(pDSV, HALgfx::CLEAR_DEPTH, 1.f, 0);

	pDeviceContext->SetPrimitiveTopology(HALgfx::PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

//------------------------------------------------------------------
void RenderPassPostprocess::Render(HALgfx::IDevice* pDevice, HALgfx::IDeviceContext* pDeviceContext)
{
	pDeviceContext->BeginEvent("RenderPassPostprocess");
	std::list<ShaderNode*>::iterator it = m_lShaderNodes.begin();
	std::list<ShaderNode*>::iterator itEnd = m_lShaderNodes.end();
	while (it != itEnd)
	{
		(*it)->Draw(pDeviceContext);
		it++;
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
	std::list<ShaderNode*>::iterator it = m_lShaderNodes.begin();
	std::list<ShaderNode*>::iterator itEnd = m_lShaderNodes.end();
	while (it != itEnd)
	{
		ShaderNode* pShaderNode = *it;
		pShaderNode->ClearDrawNodes();
		it++;
	}
}

//------------------------------------------------------------------
void RenderPassPostprocess::Setup(HALgfx::IDevice* pDevice)
{
	if (!m_bInitialized)
	{
		Initialize(pDevice);
		m_bInitialized = true;
	}

	// tonemapping
	
	std::list<ShaderNode*>::iterator itEnd = m_lShaderNodes.end();

	for (std::list<ShaderNode*>::iterator it = m_lShaderNodes.begin(); it != itEnd; it++)
	{
		ShaderNode* shaderNode = *it;
		const char* name = shaderNode->GetName();

		if (strcmp(name, "tonemapping") == 0)
		{
			DrawNode& drawNode = shaderNode->GetDrawNode("quad");

			CBufferToneMapping* pBuffer = static_cast<CBufferToneMapping*>(drawNode.m_pPSCBufferData[0]);
			pBuffer->v4Param.x = m_iDimensionX;
			pBuffer->v4Param.y = m_iDimensionY;
			pBuffer->v4Param.z = m_fIntensityLevel;
		}
	}
}

//------------------------------------------------------------------
void RenderPassPostprocess::Initialize(HALgfx::IDevice* pDevice)
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

	// tone mapping shader
	{
		char shaderName[256];
		strcpy(shaderName, "tonemapping");
		ShaderNode* pToneMappingShaderNode = new ShaderNode(shaderName);

		pToneMappingShaderNode->LoadShader();
		pToneMappingShaderNode->Create(pDevice);

		// create const buffers
		HALgfx::BufferDesc desc;
		HALgfx::SubResourceData data;
		desc.bindFlags = HALgfx::BIND_CONSTANT_BUFFER;
		desc.byteSize = sizeof(CBufferTransform);
		desc.cpuAccessFlags = HALgfx::CPU_ACCESS_WRITE;
		desc.usage = HALgfx::USAGE_DYNAMIC;
		pToneMappingShaderNode->AddConstantBuffer(desc, pDevice, HALgfx::VERTEX_SHADER);

		desc.byteSize = sizeof(CBufferToneMapping);
		pToneMappingShaderNode->AddConstantBuffer(desc, pDevice, HALgfx::PIXEL_SHADER);

		// input layout
		pToneMappingShaderNode->CreateInputLayout(2, aInputElementsDesc, pDevice);

		GPUResourceManager& gpuResourceManager = GPUResourceManager::GetInstance();
		HALgfx::ISamplerState* pSampler = gpuResourceManager.GetSamplerState(Scene::NOMIP_LINEAR_WRAP);
		pToneMappingShaderNode->SetSamplerStates(&pSampler, 1);

		m_lShaderNodes.push_back(pToneMappingShaderNode);

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
		pTonemap->v4Param.y = static_cast<float>(768);
		pTonemap->v4Param.z = 10.f;

		// framebuffer texture
		drawNodeTonemapping.AddSRV(m_pSRVHDR);

		pToneMappingShaderNode->AddDrawNode(drawNodeTonemapping);
	}

	// other shaders

}

//------------------------------------------------------------------
std::list<ShaderNode*>& RenderPassPostprocess::GetShaderNodeList()
{
	return m_lShaderNodes;
}
} // namespace Renderer
} // namespace Magnet