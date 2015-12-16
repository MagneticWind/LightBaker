#include <assert.h>
#include <stdlib.h>

#include "HALgfx\IInputLayout.h"
#include "HALgfx\IDevice.h"
#include "HALgfx\IDeviceContext.h"
#include "HALgfx\IResource.h"
#include "HALgfx\ISamplerState.h"
#include "HALgfx\IBuffer.h"
#include "HALgfx\ShaderType.h"

#include "Math\Vector3f.h"
#include "RenderManager.h"

#include "Scene\Material.h"
#include "ShaderNode.h"

namespace Magnet
{
namespace Renderer
{
//------------------------------------------------------------------
ShaderNode::ShaderNode(const char* pName) : m_iNumberOfVSConstBuffers(0), m_iNumberOfPSConstBuffers(0)
{
	strcpy(m_caShaderName, pName);

	for (int i = 0; i < HALgfx::MAX_SHADER_NUM; ++i)
	{
		m_ppShader[i] = 0;
	}

	m_pInputLayout = 0;

	for (int i = 0; i < MAX_NUMBER_BUFFERS; ++i)
	{
		m_ppVSConstBuffers[i] = 0;
		m_ppPSConstBuffers[i] = 0;
		m_aVSConstBufferSizes[i] = 0;
		m_aPSConstBufferSizes[i] = 0;
	}

	m_iNumSamplerState = 0;
	for (int i = 0; i < MAX_NUMBER_SAMPLERS; ++i)
	{
		m_ppSamplers[i] = 0;
	}

	for (int i = 0; i < HALgfx::MAX_SHADER_NUM; ++i)
	{
		m_ppFileData[i] = 0;
		m_iFileSize[i] = 0;
	}

	m_bIsLoaded = false;
}

//------------------------------------------------------------------
ShaderNode::~ShaderNode()
{
	for (int i = 0; i < HALgfx::MAX_SHADER_NUM; ++i)
	{
		if (m_ppFileData[i])
		{
			free(m_ppFileData[i]);
			m_ppFileData[i] = 0;
		}

		if (m_ppShader[i])
		{
			delete m_ppShader[i];
			m_ppShader[i] = 0;
		}
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
	}

	for (int i = 0; i < MAX_NUMBER_SAMPLERS; ++i)
	{
		m_ppSamplers[i] = 0;
	}
}

//------------------------------------------------------------------
void ShaderNode::BindDrawNodeResource(HALgfx::IDeviceContext* pDeviceContext, DrawNode& drawNode)
{
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

	// update pixel shader const buffers
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

}

//------------------------------------------------------------------
void ShaderNode::UnbindDrawNodeResource(HALgfx::IDeviceContext* pDeviceContext, DrawNode& drawNode)
{
	pDeviceContext->SetVertexBuffer(0, 0, 0, 0);
	pDeviceContext->SetIndexBuffer(0, HALgfx::FORMAT_UNKNOWN, 0);

	HALgfx::IBuffer* pBuffers[MAX_NUMBER_BUFFERS] = {0, 0 ,0 , 0, 0, 0, 0, 0};
	pDeviceContext->SetConstantBuffers(0, m_iNumberOfVSConstBuffers, HALgfx::VERTEX_SHADER, pBuffers);
	pDeviceContext->SetConstantBuffers(m_iNumberOfVSConstBuffers, m_iNumberOfPSConstBuffers, HALgfx::PIXEL_SHADER, pBuffers);

	HALgfx::IShaderResourceView* pSRVS[MAX_NUMBER_SRVS] = {0,0, 0, 0, 0, 0, 0, 0};
	pDeviceContext->SetShaderResourceViews(HALgfx::PIXEL_SHADER, 0, drawNode.m_iNumberOfSRVs, pSRVS);
}

//------------------------------------------------------------------
void ShaderNode::Draw(HALgfx::IDeviceContext* pDeviceContext)
{
	pDeviceContext->BeginEvent(m_caShaderName);

	pDeviceContext->SetInputlayout(m_pInputLayout);

	pDeviceContext->SetSamplerStates(HALgfx::PIXEL_SHADER, 0, m_iNumSamplerState, m_ppSamplers);

	for (int i = 0; i < HALgfx::MAX_SHADER_NUM; ++i)
		pDeviceContext->SetShader(static_cast<HALgfx::ShaderType>(i), m_ppShader[i]);

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

	for (int i = 0; i < HALgfx::MAX_SHADER_NUM; ++i)
		pDeviceContext->SetShader(static_cast<HALgfx::ShaderType>(i), 0);

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
void ShaderNode::AddDrawNode(DrawNode& drawNode)
{
	m_lDrawNodes.push_back(drawNode);
}

//------------------------------------------------------------------
void ShaderNode::SetSamplerStates(HALgfx::ISamplerState* pSamplerStates[], int iNumSamplerStates)
{
	m_iNumSamplerState = iNumSamplerStates;
	for (int i = 0; i < iNumSamplerStates; ++i)
	{
		m_ppSamplers[i] = pSamplerStates[i];
	}
}

//------------------------------------------------------------------
void ShaderNode::CreateInputLayout(int iNumElements, HALgfx::InputElementDesc inputElements[], HALgfx::IDevice* pDevice)
{
	m_pInputLayout = pDevice->CreateInputLayout(iNumElements, inputElements, m_iFileSize[HALgfx::VERTEX_SHADER], m_ppFileData[HALgfx::VERTEX_SHADER]);
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
void ShaderNode::Create(HALgfx::IDevice* pDevice)
{
	for (int i = 0; i < HALgfx::MAX_SHADER_NUM; ++i)
	{
		if (m_ppFileData[i] && m_iFileSize[i])
			m_ppShader[i] = pDevice->CreateShader(static_cast<HALgfx::ShaderType>(i), m_iFileSize[i], m_ppFileData[i]);
	}
}

//------------------------------------------------------------------
void* ShaderNode::CreateBuffer(int iSize, HALgfx::ShaderType eType)
{
	m_ppFileData[eType] = malloc(iSize);
	m_iFileSize[eType] = iSize;
	return m_ppFileData[eType];
}

//------------------------------------------------------------------
void ShaderNode::LoadShader()
{
	static const char SHADER_PATH[256] = "C:\\Projects\\GitHub\\LightBaker\\data\\shader\\";
	char filePath[256];
	strcpy(filePath, SHADER_PATH);
	strcat(filePath, m_caShaderName);

	// load vertex shader
	char vShaderPath[256];
	strcpy(vShaderPath, filePath);
	strcat(vShaderPath, ".v");

	FILE* pVSFile = fopen(vShaderPath, "r+b");
	if (pVSFile == 0)
	{
		printf("can't find the shader %s", vShaderPath);
		assert(0);
	}

	fseek(pVSFile, 0, SEEK_END);
	int iVSFileSize = ftell(pVSFile);
	rewind(pVSFile);

	void* pVSFileData = CreateBuffer(iVSFileSize, HALgfx::VERTEX_SHADER);
	int iResult = fread(pVSFileData, 1, iVSFileSize, pVSFile);
	if (iResult != iVSFileSize)
	{
		printf("Error reading shader file %s", pVSFileData);
		assert(0);
	}

	fclose(pVSFile);

	// load pixel shader

	char pShaderPath[256];
	strcpy(pShaderPath, filePath);
	strcat(pShaderPath, ".p");

	FILE* pPSFile = fopen(pShaderPath, "r+b");
	if (pPSFile == 0)
	{
		printf("can't find the shader %s", pShaderPath);
		assert(0);
	}

	fseek(pPSFile, 0, SEEK_END);
	int iPSFileSize = ftell(pPSFile);
	rewind(pPSFile);

	void* pPSFileData = CreateBuffer(iPSFileSize, HALgfx::PIXEL_SHADER);
	int iSize = fread(pPSFileData, 1, iPSFileSize, pPSFile);
	if (iSize != iPSFileSize)
	{
		printf("Error reading shader file %s", pShaderPath);
		assert(0);
	}

	fclose(pPSFile);

	m_bIsLoaded = true;
}

} // namespace Renderer
} // namespace Magnet