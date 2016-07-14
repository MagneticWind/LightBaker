#include <assert.h>
#include <stdlib.h>

#include "HALgfx\IInputLayout.h"
#include "HALgfx\IDevice.h"
#include "HALgfx\IDeviceContext.h"
#include "HALgfx\IResource.h"
#include "HALgfx\ISamplerState.h"
#include "HALgfx\IUnorderedAccessView.h"
#include "HALgfx\IShaderResourceView.h"
#include "HALgfx\IBuffer.h"
#include "HALgfx\IShader.h"
#include "HALgfx\ShaderType.h"
#include "HALgfx\Viewport.h"

#include "Math\Vector3f.h"
#include "RenderManager.h"

#include "Scene\Material.h"
#include "Scene\Texture.h"

#include "ShaderNode.h"

namespace Magnet
{
namespace Renderer
{
//------------------------------------------------------------------
	ShaderNode::ShaderNode(const char* pName, HALgfx::IDevice* pDevice) : m_iNumberOfVSConstBuffers(0), m_iNumberOfPSConstBuffers(0), m_iNumberOfCSConstBuffers(0)
{
	m_pShaderProgram = pDevice->CreateProgram(pName);

	m_pInputLayout = 0;
	m_iNumElements = 0;

	m_iNumTextureLabels = 0;

	for (int i = 0; i < MAX_NUMBER_BUFFERS; ++i)
	{
		m_ppVSConstBuffers[i] = 0;
		m_ppPSConstBuffers[i] = 0;
		m_ppCSConstBuffers[i] = 0;
		m_aVSConstBufferSizes[i] = 0;
		m_aPSConstBufferSizes[i] = 0;
		m_aCSConstBufferSizes[i] = 0;
	}
}

//------------------------------------------------------------------
ShaderNode::~ShaderNode()
{
	if (m_pShaderProgram)
	{
		delete m_pShaderProgram;
		m_pShaderProgram = 0;
	}

	if (m_pInputLayout)
	{
		delete m_pInputLayout;
		m_pInputLayout = 0;
	}

	for (int i = 0; i < MAX_NUMBER_BUFFERS; ++i)
	{
		if (m_ppVSConstBuffers[i])
		{
			delete m_ppVSConstBuffers[i];
			m_ppVSConstBuffers[i] = 0;
		}
		
		if (m_ppPSConstBuffers[i])
		{
			delete m_ppPSConstBuffers[i];
			m_ppPSConstBuffers[i] = 0;
		}

		if (m_ppCSConstBuffers[i])
		{
			delete m_ppCSConstBuffers[i];
			m_ppCSConstBuffers[i];
		}
	}

}

//------------------------------------------------------------------
void ShaderNode::BindDrawNodeResource(HALgfx::IDeviceContext* pDeviceContext, DrawNode& drawNode)
{
	if (drawNode.m_bSetViewport)
		pDeviceContext->SetViewPort(drawNode.mViewport);

	// vertex buffer and index buffer
	HALgfx::BufferDesc desc;
	drawNode.m_pVertexBuffer->GetDesc(desc);
	int iStride = desc.byteStride;

 	pDeviceContext->SetVertexBuffer(0, iStride, 0, drawNode.m_pVertexBuffer);
	pDeviceContext->SetIndexBuffer(drawNode.m_pIndexBuffer, HALgfx::FORMAT_R32_UINT, 0);

	HALgfx::SubResourceData data;

	// update vertex shader const buffers
	for (int i = 0; i < m_iNumberOfVSConstBuffers; ++i)
	{
		assert(drawNode.m_pVSCBufferData[i] != 0);

		pDeviceContext->Map(m_ppVSConstBuffers[i], 0, HALgfx::MAP_WRITE_DISCARD, 0, data);
		memcpy(data.pMem, drawNode.m_pVSCBufferData[i], m_aVSConstBufferSizes[i]);
		pDeviceContext->Unmap(m_ppVSConstBuffers[i], 0);
	}
	pDeviceContext->SetConstantBuffers(0, m_iNumberOfVSConstBuffers, HALgfx::VERTEX_SHADER, m_ppVSConstBuffers);

	// update pixel shader const buffers, const buffers have unique indicies in the vertex\pixel shader
	for (int i = 0; i < m_iNumberOfPSConstBuffers; ++i)
	{
		assert(drawNode.m_pPSCBufferData[i] != 0);

		pDeviceContext->Map(m_ppPSConstBuffers[i], 0, HALgfx::MAP_WRITE_DISCARD, 0, data);
		memcpy(data.pMem, drawNode.m_pPSCBufferData[i], m_aPSConstBufferSizes[i]);
		pDeviceContext->Unmap(m_ppPSConstBuffers[i], 0);
	}
	pDeviceContext->SetConstantBuffers(m_iNumberOfVSConstBuffers, m_iNumberOfPSConstBuffers, HALgfx::PIXEL_SHADER, m_ppPSConstBuffers);

	// textures
	pDeviceContext->SetShaderResourceViews(HALgfx::PIXEL_SHADER, 0, drawNode.m_iNumberOfSRVs, drawNode.m_ppSRVs);

	// samplers
	pDeviceContext->SetSamplerStates(HALgfx::PIXEL_SHADER, 0, drawNode.m_iNumberOfSamplers, drawNode.m_ppSamplers);

}

//------------------------------------------------------------------
void ShaderNode::UnbindDrawNodeResource(HALgfx::IDeviceContext* pDeviceContext, DrawNode& drawNode)
{
	pDeviceContext->SetVertexBuffer(0, 0, 0, 0);
	pDeviceContext->SetIndexBuffer(0, HALgfx::FORMAT_UNKNOWN, 0);

	HALgfx::IBuffer* pBuffers[MAX_NUMBER_BUFFERS] = {0, 0 ,0 , 0, 0, 0, 0, 0};
	pDeviceContext->SetConstantBuffers(0, m_iNumberOfVSConstBuffers, HALgfx::VERTEX_SHADER, pBuffers);
	pDeviceContext->SetConstantBuffers(m_iNumberOfVSConstBuffers, m_iNumberOfPSConstBuffers, HALgfx::PIXEL_SHADER, pBuffers);

	HALgfx::IShaderResourceView* pSRVS[MAX_NUMBER_SRVS] = {0, 0, 0, 0, 0, 0, 0, 0};
	pDeviceContext->SetShaderResourceViews(HALgfx::PIXEL_SHADER, 0, drawNode.m_iNumberOfSRVs, pSRVS);
}

//------------------------------------------------------------------
void ShaderNode::Draw(HALgfx::IDeviceContext* pDeviceContext)
{
	pDeviceContext->BeginEvent(m_pShaderProgram->GetName());

	m_pShaderProgram->SetShaders(m_iNumTextureLabels, m_textureLabels, pDeviceContext);

	pDeviceContext->SetInputlayout(m_pInputLayout);

	std::list<DrawNode>::iterator it = m_lDrawNodes.begin();
	std::list<DrawNode>::iterator itEnd = m_lDrawNodes.end();
	while(it != itEnd)
	{
		DrawNode& drawNode = *it;

		pDeviceContext->BeginEvent(drawNode.m_caName);

		BindDrawNodeResource(pDeviceContext, drawNode);
		pDeviceContext->DrawIndexed(drawNode.m_iNumberOfPrimitives * 3, 0, 0);
		UnbindDrawNodeResource(pDeviceContext, drawNode);

		pDeviceContext->EndEvent();

		it++;
	}

	pDeviceContext->SetInputlayout(0);

	pDeviceContext->EndEvent();
}


//------------------------------------------------------------------
void ShaderNode::ClearDrawNodes()
{
	std::list<DrawNode>::iterator it = m_lDrawNodes.begin();
	std::list<DrawNode>::iterator itEnd = m_lDrawNodes.end();

	while(it != itEnd)
	{
		DrawNode& drawNode = *it;

		for (int i = 0; i < drawNode.m_iNumberOfVSConstBuffers; ++i)
		{
			drawNode.DestroyCBufferData(drawNode.m_pVSCBufferData[i]);
		}

		for (int i = 0 ; i < drawNode.m_iNumberOfPSConstBuffers; ++i)
		{
			drawNode.DestroyCBufferData(drawNode.m_pPSCBufferData[i]);
		}

		it++;
	}

	m_lDrawNodes.clear();
}

//------------------------------------------------------------------
void ShaderNode::AddDrawNode(DrawNode& drawNode, HALgfx::IDevice* pDevice)
{
	static bool inputLayoutSetup = false;
	// first draw node, set up VAO, for OpenGL only
	if (!inputLayoutSetup)
	{
		pDevice->SetupInputLayout(m_pInputLayout, drawNode.m_pVertexBuffer);
		inputLayoutSetup = true;
	}
	m_lDrawNodes.push_back(drawNode);
}

//------------------------------------------------------------------
DrawNode& ShaderNode::GetDrawNode(const char* name)
{
	std::list<DrawNode> drawNodeList;
	for (std::list<DrawNode>::iterator it = m_lDrawNodes.begin(); it != m_lDrawNodes.end(); it++)
	{
		if (strcmp((*it).m_caName, name) == 0)
		{
			return *it;
		}
	}
}

//------------------------------------------------------------------
void ShaderNode::CreateInputLayout(int iNumElements, HALgfx::InputElementDesc inputElements[], int iStride, HALgfx::IDevice* pDevice)
{
	m_iNumElements = iNumElements;
	for (int i = 0; i < iNumElements; ++i)
	{
		m_inputElements[i] = inputElements[i];
	}

	m_pInputLayout = pDevice->CreateInputLayout(iNumElements, inputElements, iStride, m_pShaderProgram->GetFileSize(HALgfx::VERTEX_SHADER), m_pShaderProgram->GetFileData(HALgfx::VERTEX_SHADER));
}

//------------------------------------------------------------------
void ShaderNode::AddConstantBuffer(const HALgfx::BufferDesc& desc, HALgfx::IDevice* pDevice, HALgfx::ShaderType eType)
{
	HALgfx::SubResourceData data;
	data.pMem = 0;

	if (eType == HALgfx::VERTEX_SHADER)
	{
		m_ppVSConstBuffers[m_iNumberOfVSConstBuffers] = pDevice->CreateBuffer(desc, data);
		m_aVSConstBufferSizes[m_iNumberOfVSConstBuffers] = desc.byteSize;
		++m_iNumberOfVSConstBuffers;
	}
	else if (eType == HALgfx::PIXEL_SHADER)
	{
		m_ppPSConstBuffers[m_iNumberOfPSConstBuffers] = pDevice->CreateBuffer(desc, data);
		m_aPSConstBufferSizes[m_iNumberOfPSConstBuffers] = desc.byteSize;
		++m_iNumberOfPSConstBuffers;
	}
	else if (eType == HALgfx::COMPUTE_SHADER)
	{
		m_ppCSConstBuffers[m_iNumberOfCSConstBuffers] = pDevice->CreateBuffer(desc, data);
		m_aCSConstBufferSizes[m_iNumberOfCSConstBuffers] = desc.byteSize;
		++m_iNumberOfCSConstBuffers;
	}
}

//------------------------------------------------------------------
bool ShaderNodeExists(const char* pName, std::list<ShaderNode*>& shaderNodeList, ShaderNode** ppReturnNode)
{
	std::list<ShaderNode*>::iterator it = shaderNodeList.begin();
	std::list<ShaderNode*>::iterator itEnd = shaderNodeList.end();
	while(it != itEnd)
	{
		ShaderNode* pShaderNode = *it;
		if(strcmp(pName, pShaderNode->GetName()) == 0)
		{
			*ppReturnNode = pShaderNode;
			return true;
		}
		it++;
	}
	*ppReturnNode = 0;
	return false;
}

//------------------------------------------------------------------
void ShaderNode::Create(int iNumElements, HALgfx::InputElementDesc inputElements[], HALgfx::IDevice* pDevice)
{
	m_pShaderProgram->CreateShaders(iNumElements, inputElements, pDevice);
}

//------------------------------------------------------------------
void ShaderNode::LoadShader(HALgfx::ShaderType eType)
{
	m_pShaderProgram->LoadShader(eType);
}

//------------------------------------------------------------------
void ShaderNode::AddTextureLabel(int iLabel)
{
	m_textureLabels[m_iNumTextureLabels++] = iLabel;
}

//------------------------------------------------------------------
void ShaderNode::RunCompute(HALgfx::IDeviceContext* pDeviceContext, int iSRVCount, HALgfx::IShaderResourceView** ppSRVs,
	void* CBufferData, int iUAVCount, HALgfx::IUnorderedAccessView** ppUAVs, unsigned int uX, unsigned int uY, unsigned uZ)
{
	assert(m_iNumberOfCSConstBuffers == 1);

	pDeviceContext->SetShader(HALgfx::COMPUTE_SHADER, m_pShaderProgram->GetShader(HALgfx::COMPUTE_SHADER));
	
	HALgfx::SubResourceData data;
	pDeviceContext->Map(m_ppCSConstBuffers[0], 0, HALgfx::MAP_WRITE_DISCARD, 0, data);
	memcpy(data.pMem, CBufferData, m_aCSConstBufferSizes[0]);
	pDeviceContext->Unmap(m_ppCSConstBuffers[0], 0);
	
	pDeviceContext->SetConstantBuffers(0, m_iNumberOfCSConstBuffers, HALgfx::COMPUTE_SHADER, m_ppCSConstBuffers);
	pDeviceContext->SetShaderResourceViews(HALgfx::COMPUTE_SHADER, 0, iSRVCount, ppSRVs);
	pDeviceContext->SetUnorderedAccessViews(HALgfx::COMPUTE_SHADER, 0, iUAVCount, ppUAVs);

	pDeviceContext->ExuecuteCompute(uX, uY, uZ);

	// clear
	pDeviceContext->SetShader(HALgfx::COMPUTE_SHADER, NULL);

	HALgfx::IBuffer* pBuffers[MAX_NUMBER_BUFFERS] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	pDeviceContext->SetConstantBuffers(0, m_iNumberOfCSConstBuffers, HALgfx::COMPUTE_SHADER, pBuffers);

	HALgfx::IShaderResourceView* pSRVs[MAX_NUMBER_SRVS] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	pDeviceContext->SetShaderResourceViews(HALgfx::COMPUTE_SHADER, 0, iSRVCount, pSRVs);

	HALgfx::IUnorderedAccessView* pUAVs[MAX_NUMBER_UAVS] = { 0, 0, 0, 0 };
	pDeviceContext->SetUnorderedAccessViews(HALgfx::COMPUTE_SHADER, 0, iUAVCount, pUAVs);
}

const char* ShaderNode::GetName() const
{
	return m_pShaderProgram->GetName();
}

} // namespace Renderer
} // namespace Magnet