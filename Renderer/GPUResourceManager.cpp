#include <assert.h>

#include "HALgfx\IDevice.h"
#include "HALgfx\IBuffer.h"
#include "HALgfx\ITexture2d.h"
#include "HALgfx\IInputLayout.h"
#include "HALgfx\IResource.h"
#include "HALgfx\IShaderResourceView.h"
#include "HALgfx\ISamplerState.h"

#include "GPUResourceManager.h"
#include "Scene\Mesh.h"
#include "Scene\Texture.h"

namespace Magnet
{
namespace Renderer
{
GPUResourceManager* GPUResourceManager::ms_pInstance = 0;

//------------------------------------------------------------------
GPUResourceManager::GPUResourceManager() 
{
}

//------------------------------------------------------------------
GPUResourceManager::~GPUResourceManager()
{
	std::map<std::string, MeshResource>::iterator itMesh = m_MeshMap.begin();
	std::map<std::string, MeshResource>::iterator itMeshEnd = m_MeshMap.end();

	while (itMesh != itMeshEnd)
	{
		MeshResource& meshResource = itMesh->second;
		if (meshResource.m_pVertexBuffer)
		{
			delete meshResource.m_pVertexBuffer;
			meshResource.m_pVertexBuffer = NULL;
		}
		if (meshResource.m_pIndexBuffer)
		{
			delete meshResource.m_pIndexBuffer;
			meshResource.m_pIndexBuffer = NULL;
		}
	}

	std::map<std::string, TextureResource>::iterator itTexture = m_TextureMap.begin();
	std::map<std::string, TextureResource>::iterator itTextureEnd = m_TextureMap.end();

	while (itTexture != itTextureEnd)
	{
		TextureResource& textureResource = itTexture->second;
		if (textureResource.m_pShaderResourceView)
		{
			delete textureResource.m_pShaderResourceView;
			textureResource.m_pShaderResourceView = NULL;
		}
		if (textureResource.m_pTexture)
		{
			delete textureResource.m_pTexture;
			textureResource.m_pTexture = NULL;
		}
	}

	std::map<int, HALgfx::ISamplerState*>::iterator itSampler = m_SamplerMap.begin();
	std::map<int, HALgfx::ISamplerState*>::iterator itSamplerEnd = m_SamplerMap.end();

	while (itSampler != itSamplerEnd)
	{
		HALgfx::ISamplerState* pSampler = itSampler->second;
		if (pSampler)
		{
			delete pSampler;
			pSampler = NULL;
		}
	}
}

//------------------------------------------------------------------
GPUResourceManager& GPUResourceManager::GetInstance()
{
	assert(ms_pInstance != 0);
	return *ms_pInstance;
}

//------------------------------------------------------------------
bool GPUResourceManager::Exist()
{
	return ms_pInstance != 0;
}

//------------------------------------------------------------------
void GPUResourceManager::Initialize()
{
	assert(ms_pInstance == 0);
	ms_pInstance = new GPUResourceManager();
}

//------------------------------------------------------------------
void GPUResourceManager::Terminate()
{
	delete ms_pInstance;
	ms_pInstance = 0;

}

//------------------------------------------------------------------
void GPUResourceManager::CreateMeshResource(const Scene::Mesh* pMesh, HALgfx::IDevice* pDevice)//, void* pVertexShaderCode, int iShaderSize)
{
	std::string name(pMesh->GetMeshName());

	std::map<std::string, MeshResource>::iterator it = m_MeshMap.find(name);
	if (it == m_MeshMap.end())
	{
		MeshResource meshResource;

		// input elements
		int iSize = 0;
		Scene::VertexDecl decls[Scene::MAX_DESC_COUNT];
		pMesh->GetVetexDecls(iSize, decls);
		for (int i = 0; i < iSize; ++i)
		{
			switch (decls[i])
			{
			case Scene::POSITION:
				meshResource.AddElement(HALgfx::INPUT_SEMANTIC_POSITION, 3);
				break;
			case Scene::NORMAL:
				meshResource.AddElement(HALgfx::INPUT_SEMANTIC_NORMAL, 3);
				break;
			case Scene::TEXCOORD:
				meshResource.AddElement(HALgfx::INPUT_SEMANTIC_TEXCOORD, 2);
				break;
			default:
				assert(0);
				break;
			}
		}
		
		// vertex buffer
		HALgfx::BufferDesc desc;
		desc.bindFlags = HALgfx::BIND_VERTEX_BUFFER;
		desc.byteStride = meshResource.m_iStride;
		desc.byteSize = desc.byteStride * pMesh->GetNumVerts();
		//desc.cpuAccessFlags = HALgfx::CPU_ACCESS_READ;
		desc.usage = HALgfx::USAGE_DEFAULT;
		//desc.miscFlags
		HALgfx::SubResourceData data;
		data.pMem = const_cast<void*>(pMesh->GetVertexBufferPtr());
		meshResource.m_pVertexBuffer = pDevice->CreateBuffer(desc, data);

		// index buffer
		desc.bindFlags = HALgfx::BIND_INDEX_BUFFER;
		desc.byteStride = sizeof(unsigned int) * 3;
		desc.byteSize = desc.byteStride * pMesh->GetNumFaces();
		//desc.cpuAccessFlags = HALgfx::CPU_ACCESS_READ;
		desc.usage = HALgfx::USAGE_DEFAULT;
		data.pMem = const_cast<void*>(pMesh->GetIndexBuffePtr());
		meshResource.m_pIndexBuffer = pDevice->CreateBuffer(desc, data);

		meshResource.m_iPrimitivesCount = pMesh->GetNumPrimitives();

		m_MeshMap[name] = meshResource;
	}
}

//------------------------------------------------------------------
void GPUResourceManager::CreateTextureResource(const Scene::Texture* pTexture, HALgfx::IDevice* pDevice)
{
	TextureResource textureResource;

	std::string name(pTexture->GetName());

	std::map<std::string, TextureResource>::iterator it = m_TextureMap.find(name);
	if (it == m_TextureMap.end())
	{
		if (pTexture->GetType() == Scene::TEXTURE_TYPE_2D)
		{
			HALgfx::Texture2dDesc texDesc;
			texDesc.width = pTexture->GetWidth();
			texDesc.height = pTexture->GetHeight();

			texDesc.bindFlags = HALgfx::BIND_SHADER_RESOURCE;

			HALgfx::SubResourceData data;
			data.pMem = const_cast<void*>(pTexture->GetDataBufferPtr());
			switch (pTexture->GetFormat())
			{
			case Scene::TEXTURE_FORMAT_R8G8B8A8_UINT:
				data.uMemPitch = 4 * pTexture->GetWidth();
				texDesc.format = HALgfx::FORMAT_R8G8B8A8_UINT;
				break;
			case Scene::TEXTURE_FORMAT_R8G8B8A8_UNORM:
				data.uMemPitch = 4 * pTexture->GetWidth();
				texDesc.format = HALgfx::FORMAT_R8G8B8A8_UNORM;
				break;
			}

			textureResource.m_pTexture = pDevice->CreateTexture2d(texDesc, &data);

			HALgfx::ShaderResourceViewDesc desc;
			desc.viewDimension = HALgfx::SRV_DIMENSION_TEXTURE2D;
			desc.texSRV.mostDetailedMip = 0;
			desc.texSRV.mipLevels = texDesc.mipLevels;
			desc.format = texDesc.format;
			textureResource.m_pShaderResourceView = pDevice->CreateShaderResourceView(textureResource.m_pTexture, desc);
		}
		else if (pTexture->GetType() == Scene::TEXTURE_TYPE_CUBE)
		{
			HALgfx::Texture2dDesc texDesc;
			texDesc.width = pTexture->GetWidth();
			texDesc.height = pTexture->GetHeight();
			texDesc.arraySize = 6;
			texDesc.miscFlags = HALgfx::MISC_TEXTURECUBE;
			texDesc.bindFlags = HALgfx::BIND_SHADER_RESOURCE;
			texDesc.mipLevels = 1; // this has to be set for cubemap, it can't be 9

			HALgfx::SubResourceData data[6];

			void* pDataPtr = const_cast<void*>(pTexture->GetDataBufferPtr());
			for (int i = 0; i < 6; ++i)
			{
				switch (pTexture->GetFormat())
				{
				case Scene::TEXTURE_FORMAT_R8G8B8A8_UINT:
					data[i].uMemPitch = 4 * pTexture->GetWidth();
					data[i].uMemSlicePitch = 0;
					texDesc.format = HALgfx::FORMAT_R8G8B8A8_UINT;
					data[i].pMem = static_cast<unsigned char*>(pDataPtr)+i * 4 * pTexture->GetWidth() * pTexture->GetHeight();
					break;
				case Scene::TEXTURE_FORMAT_R8G8B8A8_UNORM:
					data[i].uMemPitch = 4 * pTexture->GetWidth();
					data[i].uMemSlicePitch = 0;
					texDesc.format = HALgfx::FORMAT_R8G8B8A8_UNORM;
					data[i].pMem = static_cast<unsigned char*>(pDataPtr) + i * 4 * pTexture->GetWidth() * pTexture->GetHeight();
					break;
				case Scene::TEXTURE_FORMAT_R32G32B32A32_FLOAT:
					data[i].uMemPitch = 16 * pTexture->GetWidth();
					data[i].uMemSlicePitch = 0;
					texDesc.format = HALgfx::FORMAT_R32G32B32A32_FLOAT;
					data[i].pMem = static_cast<float*>(pDataPtr) + i * 4 * pTexture->GetWidth() * pTexture->GetHeight();
					break;
				}
			}

			textureResource.m_pTexture = pDevice->CreateTexture2d(texDesc, data);

			HALgfx::ShaderResourceViewDesc desc;
			desc.viewDimension = HALgfx::SRV_DIMENSION_TEXTURECUBE;
			desc.texSRV.mostDetailedMip = 0;
			desc.texSRV.mipLevels = texDesc.mipLevels;
			desc.format = texDesc.format;
			textureResource.m_pShaderResourceView = pDevice->CreateShaderResourceView(textureResource.m_pTexture, desc);
		}

		// set label name
		switch (pTexture->GetLabel())
		{
		case Scene::TEXTURE_LABEL_COLOR_0:
			strcpy(textureResource.m_caLabel, "txColor0");
			break;
		case Scene::TEXTURE_LABEL_COLOR_1:
			strcpy(textureResource.m_caLabel, "txColor1");
			break;
		case Scene::TEXTURE_LABEL_NORMAL:
			strcpy(textureResource.m_caLabel, "txNormal");
			break;
		case Scene::TEXTURE_LABEL_SPECULAR:
			strcpy(textureResource.m_caLabel, "txSpecular");
			break;
		case Scene::TEXTURE_LABEL_EMISSIVE:
			strcpy(textureResource.m_caLabel, "txEmissive");
			break;
		case Scene::TEXTURE_LABEL_SKY:
			strcpy(textureResource.m_caLabel, "txSky");
			break;
		case Scene::TEXTURE_LABEL_SHADOW:
			strcpy(textureResource.m_caLabel, "txShadow");
			break;
		default:
			assert(0);
		}

		CreateSamplerState(pTexture->GetSamplerMode(), pDevice);
		textureResource.m_pSampler = GetSamplerState(pTexture->GetSamplerMode(), pDevice);

		m_TextureMap[name] = textureResource;
	}
}

/*
//------------------------------------------------------------------
void GPUResourceManager::CreateCubeTextureResource(const Texture* pTexture, HALgfx::IDevice* pDevice)
{
	static const char TEXTURE_PATH[256] = "C:\\Projects\\GitHub\\LightBaker\\data\\texture\\";
	char pPath[256];
	strcpy(pPath, TEXTURE_PATH);
	strcat(pPath, pTextureName);
	
	std::string name(pTextureName);
	std::map<std::string, TextureResource>::iterator it = m_TextureMap.find(name);
	if (it == m_TextureMap.end())
	{
		TextureResource skyTexture;

		HALgfx::ITexture2d* pTexture2d = 0;
		HALgfx::IShaderResourceView* pSRV = 0;
		pDevice->LoadCubeTextureResource(pPath, &pTexture2d, &pSRV);
		
		skyTexture.m_pTexture2D = pTexture2d;
		skyTexture.m_pShaderResourceView = pSRV;

		m_TextureMap[name] = skyTexture;
	}
}
*/

//------------------------------------------------------------------
void GPUResourceManager::CreateSamplerState(const Scene::SamplerMode mode, HALgfx::IDevice* pDevice)
{
	std::map<int, HALgfx::ISamplerState*>::iterator it = m_SamplerMap.find(mode);
	if (it == m_SamplerMap.end())
	{
		switch (mode)
		{
		case Scene::SAMPLER_MIP_LINEAR_WRAP:
		{
			// linear sampler state
			HALgfx::SamplerStateDesc desc;
			desc.addressU = HALgfx::ADDRESSMODE_WRAP;
			desc.addressV = HALgfx::ADDRESSMODE_WRAP;
			desc.addressW = HALgfx::ADDRESSMODE_WRAP;
			desc.comparisonFunc = HALgfx::COMPARISON_LESS_EQUAL;
			desc.filter = HALgfx::FILTER_MIN_MAG_MIP_LINEAR;
			desc.maxAnisotropy = 1;
			desc.maxLod = 0;
			desc.minLod = 0;
			desc.mipLodBias = 0.f;
			HALgfx::ISamplerState* pSamplerState = pDevice->CreateSamplerState(desc);

			m_SamplerMap[mode] = pSamplerState;
			break;
		}
		case Scene::SAMPLER_NOMIP_LINEAR_WRAP:
		{
			// linear sampler state
			HALgfx::SamplerStateDesc desc;
			desc.addressU = HALgfx::ADDRESSMODE_WRAP;
			desc.addressV = HALgfx::ADDRESSMODE_WRAP;
			desc.addressW = HALgfx::ADDRESSMODE_WRAP;
			desc.comparisonFunc = HALgfx::COMPARISON_LESS_EQUAL;
			desc.filter = HALgfx::FILTER_MIN_MAG_NOMIP_LINEAR;
			desc.maxAnisotropy = 1;
			desc.maxLod = 0;
			desc.minLod = 0;
			desc.mipLodBias = 0.f;
			HALgfx::ISamplerState* pSamplerState = pDevice->CreateSamplerState(desc);

			m_SamplerMap[mode] = pSamplerState;
			break;
		}
		default:
			break;
		}
	}
}

//------------------------------------------------------------------
MeshResource& GPUResourceManager::GetMeshResource(const std::string name)
{
	return m_MeshMap[name];
}

//------------------------------------------------------------------
TextureResource& GPUResourceManager::GetTextureResource(const std::string name)
{
	return m_TextureMap[name];
}

//------------------------------------------------------------------
HALgfx::ISamplerState* GPUResourceManager::GetSamplerState(int samplerMode, HALgfx::IDevice* pDevice)
{
	if (m_SamplerMap[samplerMode] == 0)
	{
		CreateSamplerState(static_cast<Scene::SamplerMode>(samplerMode), pDevice);
	}

	return m_SamplerMap[samplerMode];

}

} // namespace Renderer
} // namespace Magnet