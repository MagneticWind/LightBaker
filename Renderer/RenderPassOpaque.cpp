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
#include "Scene\Material.h"
#include "Scene\Light.h"

#include "Math\Frustum.h"
#include "RenderPassOpaque.h"
#include "ShaderNode.h"

namespace Magnet
{
namespace Renderer
{
//------------------------------------------------------------------
RenderPassOpaque::RenderPassOpaque() : m_fLightIntensityLevel(1.f)
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
	pDeviceContext->SetRasterizerState(pRState);
	pDeviceContext->SetDepthStencilState(pDSState);
	pDeviceContext->SetRenderTargetViews(1, &pRTV, pDSV);

//	pDeviceContext->ClearRenderTargetView(pRTV, Math::Vector4f(0, 0, 0, 0));
//	pDeviceContext->ClearDepthStencilView(pDSV, HALgfx::CLEAR_DEPTH, 1.f, 0);

	pDeviceContext->SetPrimitiveTopology(HALgfx::PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

//------------------------------------------------------------------
void RenderPassOpaque::Render(HALgfx::IDevice* pDevice, HALgfx::IDeviceContext* pDeviceContext)
{
	pDeviceContext->BeginEvent("RenderPassOpaque");
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
void RenderPassOpaque::Setup(HALgfx::IDevice* pDevice, int iWidth, int iHeight)
{
	Scene::Scene* pScene = Scene::SceneManager::GetInstance().GetScene();
	std::list<Scene::IRenderObject*>&renderObjectList = pScene->GetRenderObjectList();

	// camera
	const Scene::Camera* pCamera = pScene->GetCurrentCamera();
	const Math::Matrix4f& mView = pCamera->GetViewMatrix();
	const Math::Matrix4f& mProjection = pCamera->GetProjectionMatrix();

	// lights
	std::list<Scene::Light*>& lightList = pScene->GetLightList();
	Scene::Light* pMainLight = *lightList.begin();
	assert(pMainLight->GetType() == Scene::LIGHT_DIRECTIONAL);

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
							
							if (pMaterial->GetTech() & Scene::SHADOW_RECEIVER)
							{
								// cascade shadowmap

								ShaderNode* pShaderNode = NULL;
								ShaderNodeExists(shaderName, m_lShaderNodes, &pShaderNode);

								// add shader node
								if (pShaderNode == NULL)
								{
									pShaderNode = new ShaderNode(shaderName);
									pShaderNode->LoadShader(HALgfx::VERTEX_SHADER);
									pShaderNode->LoadShader(HALgfx::PIXEL_SHADER);
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
									desc.byteSize = sizeof(CBufferLights);
									pShaderNode->AddConstantBuffer(desc, pDevice, HALgfx::PIXEL_SHADER);

									desc.byteSize = sizeof(CBufferShadow);
									pShaderNode->AddConstantBuffer(desc, pDevice, HALgfx::PIXEL_SHADER);

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
									Math::Matrix4f mWorld;
									pRenderObject->GetLocalToWorldTransformation(mWorld);
									pTransform->mWorld = mWorld;
									pTransform->mView = mView;
									pTransform->mProjection = mProjection;


									const Scene::Material* pMaterialNormal = static_cast<const Scene::Material*>(pMaterial);
									CBufferMaterialNormal* pMaterialBuffer = static_cast<CBufferMaterialNormal*>(drawNode.CreateCBufferData(sizeof(CBufferMaterialNormal), HALgfx::PIXEL_SHADER));
									pMaterialNormal->GetAmbient(pMaterialBuffer->v4Ka);
									pMaterialNormal->GetDiffuse(pMaterialBuffer->v4Kd);
									pMaterialNormal->GetEmission(pMaterialBuffer->v4Ke);
									pMaterialNormal->GetSpecular(pMaterialBuffer->v4Ks);
									pMaterialNormal->GetExponent(pMaterialBuffer->v4Ks.w);

									CBufferLights* pCBufferLights = static_cast<CBufferLights*>(drawNode.CreateCBufferData(sizeof(CBufferLights), HALgfx::PIXEL_SHADER));
									Math::Vector3f v3LightDirection = pMainLight->GetDirection();
									Math::Vector3f v3LightColor = pMainLight->GetColor();
									pCBufferLights->v4DirectionalLightDir = Math::Vector4f(v3LightDirection.x, v3LightDirection.y, v3LightDirection.z, 1.f);
									pCBufferLights->v4DirectionalLightColor = Math::Vector4f(v3LightColor.x, v3LightColor.y, v3LightColor.z, 1.f);

									CBufferShadow* pShadowBuffer = static_cast<CBufferShadow*>(drawNode.CreateCBufferData(sizeof(CBufferShadow), HALgfx::PIXEL_SHADER));
									pShadowBuffer->mView = m_mShadowView;
									for (int i = 0; i < MAX_CASCADE_COUNT; ++i)
										pShadowBuffer->mProjection[i] = m_mProjection[i];
									pShadowBuffer->v4DepthRange = m_v4Range;
									
									// textures
									int iNumTextures = pSurface->GetNumTextures();
									for (int i = 0; i < iNumTextures; ++i)
									{
										const Scene::Texture* pTexture = pSurface->GetTexture(i);
										std::string texName(pTexture->GetName());
										TextureResource& texResource = gpuResourceManager.GetTextureResource(texName);
										drawNode.AddSRV(texResource.m_pShaderResourceView);
									}

									// shadowmap
									drawNode.AddSRV(m_pShadowSRV);

									// assume all textures use the first texture sampler now
									if (iNumTextures > 0)
									{
										HALgfx::ISamplerState* pSampler = gpuResourceManager.GetSamplerState(pSurface->GetTexture(0)->GetSamplerMode());
										pShaderNode->SetSamplerStates(&pSampler, 1);
									}

									pShaderNode->AddDrawNode(drawNode);
								}
							}
							else
							{

								ShaderNode* pShaderNode = NULL;
								ShaderNodeExists(shaderName, m_lShaderNodes, &pShaderNode);

								// add shader node
								if (pShaderNode == NULL)
								{
									pShaderNode = new ShaderNode(shaderName);
									pShaderNode->LoadShader(HALgfx::VERTEX_SHADER);
									pShaderNode->LoadShader(HALgfx::PIXEL_SHADER);
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
									Math::Matrix4f mWorld;
									pRenderObject->GetLocalToWorldTransformation(mWorld);
									pTransform->mWorld = mWorld;
									pTransform->mView = mView;
									pTransform->mProjection = mProjection;


									const Scene::Material* pMaterialNormal = static_cast<const Scene::Material*>(pMaterial);
									CBufferMaterialNormal* pMaterialBuffer = static_cast<CBufferMaterialNormal*>(drawNode.CreateCBufferData(sizeof(CBufferMaterialNormal), HALgfx::PIXEL_SHADER));
									pMaterialNormal->GetAmbient(pMaterialBuffer->v4Ka);
									pMaterialNormal->GetDiffuse(pMaterialBuffer->v4Kd);
									pMaterialNormal->GetEmission(pMaterialBuffer->v4Ke);
									pMaterialNormal->GetSpecular(pMaterialBuffer->v4Ks);
									pMaterialNormal->GetExponent(pMaterialBuffer->v4Ks.w);

									CBufferLights* pCBufferLights = static_cast<CBufferLights*>(drawNode.CreateCBufferData(sizeof(CBufferLights), HALgfx::PIXEL_SHADER));
									Math::Vector3f v3LightDirection = pMainLight->GetDirection();
									Math::Vector3f v3LightColor = pMainLight->GetColor();
									pCBufferLights->v4DirectionalLightDir = Math::Vector4f(v3LightDirection.x, v3LightDirection.y, v3LightDirection.z, 1.f);
									pCBufferLights->v4DirectionalLightColor = Math::Vector4f(v3LightColor.x, v3LightColor.y, v3LightColor.z, 1.f);

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
							}


							break;
						}
					}
				}
			}
		}
	}
}

void RenderPassOpaque::SetShadowParameters(Math::Matrix4f* mShadowView, Math::Matrix4f* mProjection, HALgfx::IShaderResourceView** pShadowmapSRV, Math::Vector4f* pRange)
{
	m_mShadowView = *mShadowView;

	for (int i = 0; i < MAX_CASCADE_COUNT; ++i)
	{
		m_mProjection[i] = mProjection[i];
	}
	m_v4Range = *pRange;
	m_pShadowSRV = *pShadowmapSRV;
}

} // namespace Renderer
} // namespace Magnet