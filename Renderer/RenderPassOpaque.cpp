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
#include "Scene\Material.h"

#include "Math\Frustum.h"
#include "Memory.h"
#include "RenderPassOpaque.h"
#include "ShaderNode.h"

namespace Magnet
{
namespace Renderer
{
//------------------------------------------------------------------
RenderPassOpaque::RenderPassOpaque()
{
}

//------------------------------------------------------------------
RenderPassOpaque::~RenderPassOpaque()
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
void RenderPassOpaque::SetRenderState(HALgfx::IDeviceContext* pDeviceContext, const HALgfx::ViewPort& viewPort,
	HALgfx::IRenderTargetView* pRTV, HALgfx::IDepthStencilView* pDSV, HALgfx::IRasterizerState* pRState, HALgfx::IDepthStencilState* pDSState)
{
	pDeviceContext->SetViewPort(viewPort);
	pDeviceContext->SetRenderTargetViews(1, &pRTV, pDSV);
	pDeviceContext->SetRasterizerState(pRState);
	pDeviceContext->SetDepthStencilState(pDSState);

	//	pDeviceContext->ClearRenderTargetView(pRTV, Math::Vector4f(0, 0, 0, 0));
	//	pDeviceContext->ClearDepthStencilView(pDSV, HALgfx::CLEAR_DEPTH, 1.f, 0);

	pDeviceContext->SetPrimitiveTopology(HALgfx::PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

//------------------------------------------------------------------
void RenderPassOpaque::Render(HALgfx::IDevice* pDevice, HALgfx::IDeviceContext* pDeviceContext)
{
	std::list<ShaderNode*>::iterator it = m_lShaderNodes.begin();
	std::list<ShaderNode*>::iterator itEnd = m_lShaderNodes.end();
	while (it != itEnd)
	{
		(*it)->Draw(pDeviceContext);
		it++;
	}
}

//------------------------------------------------------------------
PassType RenderPassOpaque::GetType()
{
	return PASS_OPAQUE;
}

//------------------------------------------------------------------
void RenderPassOpaque::ClearDrawNodes()
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
void RenderPassOpaque::Setup(HALgfx::IDevice* pDevice)
{
	Scene::Scene* pScene = Scene::SceneManager::GetInstance().GetScene();
	std::list<Scene::IRenderObject*>&renderObjectList = pScene->GetRenderObjectList();

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
			if (pNormalRenderObject != NULL)
			{
				const std::list<Scene::Surface*>& surfaceList = pNormalRenderObject->GetSurfaceList();

				std::list<Scene::Surface*>::const_iterator itEnd = surfaceList.end();

				for (std::list<Scene::Surface*>::const_iterator it = surfaceList.begin(); it != itEnd; ++it)
				{
					const Scene::Surface* pSurface = *it;

					const Scene::IMaterial* pMaterial = pSurface->GetMaterial();
					switch (pMaterial->GetType())
					{
					case Scene::MATERIAL_NORMAL:
						{
							char shaderName[256];
							strcpy(shaderName, "norm");

							char shaderTech[256];
							pMaterial->GetTechString(shaderTech);

							strcat(shaderName, shaderTech);

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

								// material buffer
								desc.byteSize = sizeof(CBufferMaterialNormal);
								pShaderNode->AddConstantBuffer(desc, pDevice, HALgfx::PIXEL_SHADER);

								// c buffer that's shared by draw nodes: like light source parameters,
								// shadow map related parameters......
								desc.byteSize = sizeof(CBufferLights);
								pShaderNode->AddConstantBuffer(desc, pDevice, HALgfx::PIXEL_SHADER);

								m_lShaderNodes.push_back(pShaderNode);
							}
							
							// add drawnode
							{
								// add the current surface as a draw node
								DrawNode drawNode;

								const char* meshName = pSurface->GetGeometry()->GetName();

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

								const Scene::Material* pMaterialNormal = static_cast<const Scene::Material*>(pMaterial);
								CBufferMaterialNormal* pMaterialBuffer = static_cast<CBufferMaterialNormal*>(drawNode.CreateCBufferData(sizeof(CBufferMaterialNormal), HALgfx::PIXEL_SHADER));
								pMaterialNormal->GetAmbient(pMaterialBuffer->v4Ka);
								pMaterialNormal->GetDiffuse(pMaterialBuffer->v4Kd);
								pMaterialNormal->GetEmission(pMaterialBuffer->v4Ke);
								pMaterialNormal->GetSpecular(pMaterialBuffer->v4Ks);
								pMaterialNormal->GetExponent(pMaterialBuffer->v4Ks.w);

								/*CBufferOpaque* pCBufferOpaque = static_cast<CBufferOpaque*>(drawNode.CreateCBufferData(sizeof(CBufferOpaque), HALgfx::PIXEL_SHADER));
								pCBufferOpaque->v4EyePosition = Math::Vector4f(v3EyePosition.x, v3EyePosition.y, v3EyePosition.z, 1.f);
								Math::Vector3f v3LightDirection, v3LightColor;
								RenderManager::GetInstance().GetMajorLightData(v3LightDirection, v3LightColor);
								pCBufferOpaque->v4SunDirection = Math::Vector4f(v3LightDirection.x, v3LightDirection.y, v3LightDirection.z, 1.f);
								pCBufferOpaque->v4SunColor = Math::Vector4f(v3LightColor.x, v3LightColor.y, v3LightColor.z, 1.f);
								RenderManager::GetInstance().GetSHCubemap(pCBufferOpaque->v4SHCoeffs, 7);*/

								// textures
								int iNumTextures = pSurface->GetNumTextures();
								for (int i = 0; i < iNumTextures; ++i)
								{
									const Scene::Texture* pTexture = pSurface->GetTexture(i);
									std::string texName(pTexture->GetName());
									TextureResource& texResource = gpuResourceManager.GetTextureResource(texName);
									drawNode.AddSRV(texResource.m_pShaderResourceView);
								}

								// assume all textures use the first texture sampler now
								if (iNumTextures > 0)
								{
									HALgfx::ISamplerState* pSampler = gpuResourceManager.GetSamplerState(pSurface->GetTexture(0)->GetSamplerMode());
									pShaderNode->SetSamplerStates(&pSampler, 1);
								}

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
std::list<ShaderNode*>& RenderPassOpaque::GetShaderNodeList()
{
	return m_lShaderNodes;
}
} // namespace Renderer
} // namespace Magnet