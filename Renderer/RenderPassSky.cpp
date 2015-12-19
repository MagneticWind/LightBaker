#include <assert.h>

#include "CBufferDefs.h"
#include "GPUResourceManager.h"

#include "HALgfx\IBuffer.h"
#include "HALgfx\IDevice.h"
#include "HALgfx\IDeviceContext.h"
#include "HALgfx\ISamplerState.h"

#include "Scene\IRenderObject.h"
#include "Scene\RenderObject.h"
#include "Scene\SceneManager.h"
#include "Scene\Scene.h"
#include "Scene\Surface.h"
#include "Scene\MaterialSky.h"

#include "Math\Frustum.h"
#include "RenderPassSky.h"
#include "ShaderNode.h"

namespace Magnet
{
namespace Renderer
{
//------------------------------------------------------------------
RenderPassSky::RenderPassSky()
{
}

//------------------------------------------------------------------
RenderPassSky::~RenderPassSky()
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
}

//------------------------------------------------------------------
void RenderPassSky::SetRenderState(HALgfx::IDeviceContext* pDeviceContext, const HALgfx::ViewPort& viewPort,
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
void RenderPassSky::Render(HALgfx::IDevice* pDevice, HALgfx::IDeviceContext* pDeviceContext)
{
	pDeviceContext->BeginEvent("RenderPassSky");
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
PassType RenderPassSky::GetType()
{
	return PASS_SKY;
}

//------------------------------------------------------------------
void RenderPassSky::ClearDrawNodes()
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
void RenderPassSky::Setup(HALgfx::IDevice* pDevice)
{
	Scene::Scene* pScene = Scene::SceneManager::GetInstance().GetScene();
	std::list<Scene::IRenderObject*>&renderObjectList = pScene->GetRenderObjectList();

	// camera
	const Scene::Camera* pCamera = pScene->GetCurrentCamera();
	const Math::Matrix4f& mView = pCamera->GetViewMatrix();
	const Math::Matrix4f& mProjection = pCamera->GetProjectionMatrix();

	std::list<Scene::IRenderObject*>::iterator itEnd = renderObjectList.end();
	for (std::list<Scene::IRenderObject*>::iterator it = renderObjectList.begin(); it != itEnd; ++it)
	{
		Scene::IRenderObject* pRenderObject = *it;

		if (pRenderObject->GetType() == Scene::RENDEROBJECT_NORMAL)
		{
			Scene::RenderObject* pNormalRenderObject = static_cast<Scene::RenderObject*>(pRenderObject);
			if (pRenderObject != NULL)
			{
				const std::list<Scene::Surface*>& surfaceList = pNormalRenderObject->GetSurfaceList();

				std::list<Scene::Surface*>::const_iterator itEnd = surfaceList.end();

				for (std::list<Scene::Surface*>::const_iterator it = surfaceList.begin(); it != itEnd; ++it)
				{
					const Scene::Surface* pSurface = *it;

					const Scene::IMaterial* pMaterial = pSurface->GetMaterial();
					switch (pMaterial->GetType())
					{
					case Scene::MATERIAL_SKY:
					{
						char shaderName[256];
						strcpy(shaderName, "sky");

						ShaderNode* pShaderNode = NULL;
						ShaderNodeExists(shaderName, m_lShaderNodes, &pShaderNode);

						// add shader node
						if (pShaderNode == NULL)
						{
							pShaderNode = new ShaderNode(shaderName);
							pShaderNode->LoadShader();
							pShaderNode->Create(pDevice);

							// create const buffers
							HALgfx::BufferDesc desc;
							HALgfx::SubResourceData data;

							desc.bindFlags = HALgfx::BIND_CONSTANT_BUFFER;
							desc.byteSize = sizeof(CBufferTransform);
							desc.cpuAccessFlags = HALgfx::CPU_ACCESS_WRITE;
							desc.usage = HALgfx::USAGE_DYNAMIC;

							pShaderNode->AddConstantBuffer(desc, pDevice, HALgfx::VERTEX_SHADER);

							// input layout, it requires v shader byte code, and input elements from mesh
							const char* meshName = pSurface->GetGeometry()->GetName();
							GPUResourceManager& gpuResourceManager = GPUResourceManager::GetInstance();
							MeshResource& meshResource = gpuResourceManager.GetMeshResource(std::string(meshName));
							pShaderNode->CreateInputLayout(meshResource.m_iNumElements, meshResource.m_aInputElementsDesc, pDevice);

							m_lShaderNodes.push_back(pShaderNode);
						}

						// add drawnode
						{
							// add the current surface as a draw node
							DrawNode drawNode;

							const char* meshName = pSurface->GetGeometry()->GetName();

							strcpy(drawNode.m_caName, meshName);

							GPUResourceManager& gpuResourceManager = GPUResourceManager::GetInstance();
							MeshResource& meshResource = gpuResourceManager.GetMeshResource(std::string(meshName));

							// vertex buffer and index buffer
							drawNode.m_pVertexBuffer = meshResource.m_pVertexBuffer;
							drawNode.m_pIndexBuffer = meshResource.m_pIndexBuffer;
							drawNode.m_iNumberOfPrimitives = meshResource.m_iPrimitivesCount;

							// const buffers
							CBufferTransform* pTransform = static_cast<CBufferTransform*>(drawNode.CreateCBufferData(sizeof(CBufferTransform), HALgfx::VERTEX_SHADER));
							pRenderObject->GetLocalToWorldTransformation(pTransform->mWorld);
							pTransform->mView = mView;
							pTransform->mProjection = mProjection;

							// cubemap
							const Scene::MaterialSky* pSkyMaterial = static_cast<const Scene::MaterialSky*>(pMaterial);
							std::string texName(pSkyMaterial->GetMapName());
							TextureResource& texResource = gpuResourceManager.GetTextureResource(texName);
							drawNode.AddSRV(texResource.m_pShaderResourceView);

							// assume all textures use the first texture sampler now
							gpuResourceManager.CreateSamplerState(Scene::NOMIP_LINEAR_WRAP, pDevice);
							HALgfx::ISamplerState* pSampler = gpuResourceManager.GetSamplerState(Scene::NOMIP_LINEAR_WRAP);
							pShaderNode->SetSamplerStates(&pSampler, 1);

							pShaderNode->AddDrawNode(drawNode);
						}

						break;
						}
					}
				}
			}
		}
	}
}

//------------------------------------------------------------------
std::list<ShaderNode*>& RenderPassSky::GetShaderNodeList()
{
	return m_lShaderNodes;
}
} // namespace Renderer
} // namespace Magnet