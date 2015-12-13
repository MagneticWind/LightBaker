#include <assert.h>

#include "GPUResourceManager.h"

#include "HALgfx\RenderSystem_plat.h"
#include "IRenderPass.h"

#include "RenderManager.h"
#include "RenderPassOpaque.h"

#include "Scene\IRenderObject.h"
#include "Scene\RenderObject.h"
#include "Scene\SceneManager.h"
#include "Scene\Scene.h"
#include "Scene\Surface.h"
#include "Scene\Mesh.h"
#include "Scene\Texture.h"

namespace Magnet
{
namespace Renderer
{
	RenderManager* RenderManager::ms_pInstance = 0;

	//------------------------------------------------------------------
	RenderManager::RenderManager() : m_bPostProcessResourcesCreated(false)
	{
		for (int i = 0; i < PASS_NUMBER; ++i)
		{
			m_pRenderPasses[i] = 0;
		}
	}

	//------------------------------------------------------------------
	RenderManager::~RenderManager()
	{
		for (int i = 0; i < PASS_NUMBER; ++i)
		{
			delete m_pRenderPasses[i];
			m_pRenderPasses[i] = 0;
		}
	}

	//------------------------------------------------------------------
	RenderManager& RenderManager::GetInstance()
	{
		assert(ms_pInstance != 0);
		return *ms_pInstance;
	}

	//------------------------------------------------------------------
	void RenderManager::Initialize(int iWidth, int iHeight, void* pWindowHandle)
	{
		// initializa HALRenderSystem
		HALgfx::RenderSystem::Initialize();
		HALgfx::RenderSystem::GetInstance().InitializeSystem(iWidth, iHeight, pWindowHandle);

		assert(ms_pInstance == 0);

		ms_pInstance = new RenderManager();

		ms_pInstance->SetupRenderPasses();

		ms_pInstance->SetFrameBufferDimension(iWidth, iHeight);

	}

	//------------------------------------------------------------------
	void RenderManager::SetupRenderPasses()
	{
		m_pRenderPasses[PASS_OPAQUE] = new RenderPassOpaque();
	}

	//------------------------------------------------------------------
	bool RenderManager::Exist()
	{
		return ms_pInstance != 0;
	}

	//------------------------------------------------------------------
	void RenderManager::Terminate()
	{
		delete ms_pInstance;
		ms_pInstance = 0;

		HALgfx::RenderSystem::Terminate();
	}

	//------------------------------------------------------------------
	HALgfx::IDevice* RenderManager::GetDevice()
	{
		return HALgfx::RenderSystem::GetInstance().GetDevice();
	}

	//------------------------------------------------------------------
	HALgfx::IDeviceContext* RenderManager::GetDeviceContext()
	{
		return HALgfx::RenderSystem::GetInstance().GetDeviceContext();
	}

	//------------------------------------------------------------------
	HALgfx::IRenderTargetView* RenderManager::GetFrameBufferRTV()
	{
		return HALgfx::RenderSystem::GetInstance().GetFrameBufferRTV();
	}

	//------------------------------------------------------------------
	HALgfx::IRenderTargetView* RenderManager::GetFrameBufferRTVHDR()
	{
		return HALgfx::RenderSystem::GetInstance().GetFrameBufferRTVHDR();
	}

	//------------------------------------------------------------------
	HALgfx::IShaderResourceView* RenderManager::GetFrameBufferSRVHDR()
	{
		return HALgfx::RenderSystem::GetInstance().GetFrameBufferSRVHDR();
	}

	//------------------------------------------------------------------
	HALgfx::IDepthStencilView* RenderManager::GetFrameBufferDSV()
	{
		return HALgfx::RenderSystem::GetInstance().GetFrameBufferDSV();
	}

	//------------------------------------------------------------------
	HALgfx::IRasterizerState* RenderManager::GetRasterizerState()
	{
		return HALgfx::RenderSystem::GetInstance().GetRasterizerState();
	}

	//------------------------------------------------------------------
	HALgfx::ViewPort RenderManager::GetViewPort()
	{
		return HALgfx::RenderSystem::GetInstance().GetViewPort();
	}

	//------------------------------------------------------------------
	HALgfx::IDepthStencilState* RenderManager::GetDepthStencilState()
	{
		return HALgfx::RenderSystem::GetInstance().GetDepthStencilState();
	}

	//------------------------------------------------------------------
	IRenderPass* RenderManager::GetPass(PassType eType)
	{
		return m_pRenderPasses[eType];
	}

	//------------------------------------------------------------------
	void RenderManager::SetFrameBufferDimension(int iWidth, int iHeight)
	{
		m_iWidth = iWidth;
		m_iHeight = iHeight;
	}

	//------------------------------------------------------------------
	void RenderManager::GetFrameBufferDimension(int& iWidth, int& iHeight) const
	{
		iWidth = m_iWidth;
		iHeight = m_iHeight;
	}

	//------------------------------------------------------------------
	void RenderManager::RenderOneFrame()
	{
		// create gpu resources for meshes, textures
		UpdateRenderResources();

		// create shader related resources, setup shader nodes, draw nodes
		UpdateRenderPasses();

		HALgfx::RenderSystem& renderSystem = HALgfx::RenderSystem::GetInstance();
		HALgfx::IDevice* pDevice = renderSystem.GetDevice();
		HALgfx::IDeviceContext* pDeviceContext = renderSystem.GetDeviceContext();

	/*	if (m_bShadowResourcesCreated == false)
		{
			m_pShadowShader = new Shader("shadow");
			m_callBackLoadShader(m_pShadowShader);
			m_pShadowShader->Create(pDevice);

			m_bShadowResourcesCreated = true;
		}*/

		//if (m_bPostProcessResourcesCreated == false)
		//{
		//	m_pQuadMesh = new Scene::Mesh("quad.ply");
		//	m_callBackLoadMesh(m_pQuadMesh);

		//	m_pTonemapShader = new Shader("tonemap");
		//	m_callBackLoadShader(m_pTonemapShader);
		//	m_pTonemapShader->Create(pDevice);

		//	const void* pShaderCode = m_pTonemapShader->GetBufferPtr(HALgfx::VERTEX_SHADER);
		//	int iSize = m_pTonemapShader->GetBufferSize(HALgfx::VERTEX_SHADER);
		//	m_pQuadMesh->CreateHALResources(pShaderCode, iSize, pDevice);

		//	RenderPassTonemap* pTonemapPass = static_cast<RenderPassTonemap*>(m_pRenderPasses[PASS_TONEMAP]);
		//	pTonemapPass->SetMesh(m_pQuadMesh);
		//	pTonemapPass->SetShader(m_pTonemapShader);
		//	pTonemapPass->SetFrameBufferDimension(m_iWidth, m_iHeight);
		//	pTonemapPass->SetFrameBufferSRVHDR(HALgfx::RenderSystem::GetInstance().GetFrameBufferSRVHDR());

		//	m_bPostProcessResourcesCreated = true;
		//}

		HALgfx::ViewPort viewPort = renderSystem.GetViewPort();
		HALgfx::IRenderTargetView* pRTV = renderSystem.GetFrameBufferRTV();
		HALgfx::IRenderTargetView* pHDRRTV = renderSystem.GetFrameBufferRTVHDR();
		HALgfx::IDepthStencilView* pDSV = renderSystem.GetFrameBufferDSV();
		HALgfx::IDepthStencilState* pDSVStateEnabled = renderSystem.GetDepthStencilState();
		HALgfx::IDepthStencilState* pDSVStateDisabled = renderSystem.GetDepthStencilStateDisabled();
		HALgfx::IRasterizerState* pRState = renderSystem.GetRasterizerState();

		/*IRenderPass* pSkyboxPass = m_pRenderPasses[PASS_SKYBOX];
		pSkyboxPass->SetRenderState(pDeviceContext, viewPort, pHDRRTV, pDSV, pRState, pDSVStateDisabled);
		pSkyboxPass->Render(pDevice, pDeviceContext);
		pSkyboxPass->ClearDrawNodes();

		IRenderPass* pShadowPass = m_pRenderPasses[PASS_SHADOW];
		pShadowPass->SetRenderState(pDeviceContext, viewPort, 0, 0, pRState, pDSVStateDisabled);
		pShadowPass->Setup(pDevice);
		pShadowPass->Render(pDevice, pDeviceContext);
		pShadowPass->ClearDrawNodes();*/

		IRenderPass* pOpaquePass = m_pRenderPasses[PASS_OPAQUE];
		pOpaquePass->SetRenderState(pDeviceContext, viewPort, pHDRRTV, pDSV, pRState, pDSVStateEnabled);
		pOpaquePass->Render(pDevice, pDeviceContext);
		pOpaquePass->ClearDrawNodes();

		/*IRenderPass* pTonemapPass = m_pRenderPasses[PASS_TONEMAP];
		pTonemapPass->SetRenderState(pDeviceContext, viewPort, pRTV, pDSV, pRState, pDSVStateDisabled);
		pTonemapPass->Setup(pDevice);
		pTonemapPass->Render(pDevice, pDeviceContext);*/

		renderSystem.Present();
	}

	void RenderManager::UpdateRenderResources()
	{
		HALgfx::RenderSystem& renderSystem = HALgfx::RenderSystem::GetInstance();
		HALgfx::IDevice* pDevice = renderSystem.GetDevice();

		Scene::Scene* pScene = Scene::SceneManager::GetInstance().GetScene();
		std::list<Scene::IRenderObject*>&renderObjectList = pScene->GetRenderObjectList();

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
						
						const Scene::IGeometry* pGeo = pSurface->GetGeometry();
						if (pGeo->GetGeometryType() == Scene::GEOMETRY_MESH)
						{
							const Scene::Mesh* pMesh = static_cast<const Scene::Mesh*>(pGeo);

							GPUResourceManager::GetInstance().CreateMeshResource(pMesh, pDevice);
						}

						const int iTextureCount = pSurface->GetNumTextures();
						for (int i = 0; i < iTextureCount; ++i)
						{
							const Scene::Texture* pTexture = pSurface->GetTexture(i);

							GPUResourceManager::GetInstance().CreateTextureResource(pTexture, pDevice);
						}

					}
				}
			}
		}
	}

	//------------------------------------------------------------------
	void RenderManager::UpdateRenderPasses()
	{
		HALgfx::RenderSystem& renderSystem = HALgfx::RenderSystem::GetInstance();
		HALgfx::IDevice* pDevice = renderSystem.GetDevice();
		for (int i = 0; i < PASS_NUMBER; ++i)
		{
			m_pRenderPasses[i]->Setup(pDevice);
		}
	}

	//------------------------------------------------------------------
	void RenderManager::SetMajorLightData(const Math::Vector3f& v3Direction, const Math::Vector3f& v3Color)
	{
		m_v3MajorLightDirection = v3Direction;
		m_v3MajorLightColor = v3Color;
	}

	//------------------------------------------------------------------
	void RenderManager::GetMajorLightData(Math::Vector3f& v3Direction, Math::Vector3f& v3Color) const
	{
		v3Direction = m_v3MajorLightDirection;
		v3Color = m_v3MajorLightColor;
	}

//------------------------------------------------------------------
void RenderManager::SetLoadShaderCallBack(CallBackLoadShader callBackLoadShader)
{
	m_callBackLoadShader = callBackLoadShader;
}

//------------------------------------------------------------------
void RenderManager::SetLoadMeshCallBack(CallBackLoadMesh callBackLoadMesh)
{
	m_callBackLoadMesh = callBackLoadMesh;
}

//------------------------------------------------------------------
/*void RenderManager::LoadTextureResource(const char* pPath, Texture* pTexture)
{
	HALgfx::ITexture2d* pTexture2d = 0;
	HALgfx::IShaderResourceView* pSRV = 0;
	HALgfx::RenderSystem::GetInstance().LoadCubeTextureResource(pPath, &pTexture2d, &pSRV);
	pTexture->SetResource(pTexture2d);
	pTexture->SetSRV(pSRV);
	pTexture->SetLoaded(true);
}*/

//------------------------------------------------------------------
void RenderManager::GetSHCubemap(Math::Vector4f shCoeffs[], int iNumCoeffs)
{
	float faSHRed[9] = { 0.28209481, -0.48860252, 0, 0, 0, 0, -0.31539157, 0, -0.54627424 };
	float faSHGreen[9] = { 0.28209481, -0.48860252, 0, 0, 0, 0, -0.31539157, 0, -0.54627424 };
	float faSHBlue[9] = { 0.28209481, -0.48860252, 0, 0, 0, 0, -0.31539157, 0, -0.54627424 };
	//HALgfx::RenderSystem::GetInstance().GetSHFromCubemap(faSHRed, faSHGreen, faSHBlue);

	shCoeffs[0] = Math::Vector4f(faSHRed[0], faSHRed[1], faSHRed[2], faSHRed[3]);
	shCoeffs[1] = Math::Vector4f(faSHRed[4], faSHRed[5], faSHRed[6], faSHRed[7]);
	shCoeffs[2] = Math::Vector4f(faSHRed[8], faSHGreen[0], faSHGreen[1], faSHGreen[2]);
	shCoeffs[3] = Math::Vector4f(faSHGreen[3], faSHGreen[4], faSHGreen[5], faSHGreen[6]);
	shCoeffs[4] = Math::Vector4f(faSHGreen[7], faSHGreen[8], faSHBlue[0], faSHBlue[1]);
	shCoeffs[5] = Math::Vector4f(faSHBlue[2], faSHBlue[3], faSHBlue[4], faSHBlue[5]);
	shCoeffs[6] = Math::Vector4f(faSHBlue[6], faSHBlue[7], faSHBlue[8], 0.f);
}
} // namespace Renderer
} // namespace Magnet