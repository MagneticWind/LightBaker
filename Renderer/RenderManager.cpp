
#include <assert.h>

#include "Config.h"

#include "GPUResourceManager.h"

#ifdef USE_DIRECTX
#include "HALgfx\RenderSystem_plat.h"
#else
#include "HALgfx\RenderSystem_gl.h"
#endif

#include "IRenderPass.h"

#include "RenderManager.h"
#include "RenderPassDepth.h"
#include "RenderPassOpaque.h"
#include "RenderPassPostprocess.h"
#include "RenderPassSky.h"
#include "RenderPassShadow.h"

#include "Scene\IRenderObject.h"
#include "Scene\RenderObject.h"
#include "Scene\SceneManager.h"
#include "Scene\Scene.h"
#include "Scene\Surface.h"
#include "Scene\Mesh.h"
#include "Scene\Texture.h"
#include "Scene\MaterialSky.h"

namespace Magnet
{
namespace Renderer
{
	RenderManager* RenderManager::ms_pInstance = 0;
	HALgfx::IRenderSystem* RenderManager::ms_pRenderSystem = 0;

	//------------------------------------------------------------------
	RenderManager::RenderManager() : m_bPostProcessResourcesCreated(false)
	{
		for (int i = 0; i < PASS_NUMBER; ++i)
		{
			m_pRenderPasses[i] = 0;
		}

		m_iPassCount = 0;
	}

	//------------------------------------------------------------------
	RenderManager::~RenderManager()
	{
		for (int i = 0; i < m_iPassCount; ++i)
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
		// initialize HALRenderSystem

#ifdef USE_DIRECTX
		HALgfx::RenderSystem::Initialize();
		HALgfx::RenderSystem::GetInstance()->InitializeSystem(iWidth, iHeight, pWindowHandle);
		ms_pRenderSystem = HALgfx::RenderSystem::GetInstance();
#else
		HALgfx::GLRenderSystem::Initialize();
		HALgfx::GLRenderSystem::GetInstance()->InitializeSystem(iWidth, iHeight, pWindowHandle);
		ms_pRenderSystem = HALgfx::GLRenderSystem::GetInstance();
#endif

		GPUResourceManager::Initialize();

		assert(ms_pInstance == 0);

		ms_pInstance = new RenderManager();

		ms_pInstance->SetupRenderPasses();

		ms_pInstance->SetFrameBufferDimension(iWidth, iHeight);

	}

	//------------------------------------------------------------------
	void RenderManager::SetupRenderPasses()
	{
#ifdef USE_DIRECTX
		m_pRenderPasses[m_iPassCount++] = new RenderPassDepth();
		m_pRenderPasses[m_iPassCount++] = new RenderPassSky();
		m_pRenderPasses[m_iPassCount++] = new RenderPassShadow();
		m_pRenderPasses[m_iPassCount++] = new RenderPassOpaque();
		m_pRenderPasses[m_iPassCount++] = new RenderPassPostprocess();
#else
		m_pRenderPasses[m_iPassCount++] = new RenderPassOpaque();
#endif
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

		GPUResourceManager::Terminate();
		ms_pRenderSystem->TerminateSystem();
	}

	//------------------------------------------------------------------
	HALgfx::IDevice* RenderManager::GetDevice()
	{
		return ms_pRenderSystem->GetDevice();
	}

	//------------------------------------------------------------------
	HALgfx::IDeviceContext* RenderManager::GetDeviceContext()
	{
		return ms_pRenderSystem->GetDeviceContext();
	}

	//------------------------------------------------------------------
	HALgfx::IDeviceContext* RenderManager::GetDeferredDeviceContext()
	{
		return ms_pRenderSystem->GetDeferredDeviceContext();
	}

	//------------------------------------------------------------------
	HALgfx::IRenderTargetView* RenderManager::GetFrameBufferRTV()
	{
		return ms_pRenderSystem->GetFrameBufferRTV();
	}

	//------------------------------------------------------------------
	HALgfx::IRenderTargetView* RenderManager::GetFrameBufferRTVHDR()
	{
		return ms_pRenderSystem->GetFrameBufferRTVHDR();
	}

	//------------------------------------------------------------------
	HALgfx::IShaderResourceView* RenderManager::GetFrameBufferSRVHDR()
	{
		return ms_pRenderSystem->GetFrameBufferSRVHDR();
	}

	//------------------------------------------------------------------
	HALgfx::IShaderResourceView* RenderManager::GetDepthSRV()
	{
		return ms_pRenderSystem->GetDepthSRV();
	}

	//------------------------------------------------------------------
	HALgfx::IDepthStencilView* RenderManager::GetFrameBufferDSV()
	{
		return ms_pRenderSystem->GetFrameBufferDSV();
	}

	//------------------------------------------------------------------
	HALgfx::IRasterizerState* RenderManager::GetRasterizerState()
	{
		return ms_pRenderSystem->GetRasterizerState();
	}

	//------------------------------------------------------------------
	HALgfx::ViewPort RenderManager::GetViewPort()
	{
		return ms_pRenderSystem->GetViewPort();
	}

	//------------------------------------------------------------------
	HALgfx::IDepthStencilState* RenderManager::GetDepthStencilState()
	{
		return ms_pRenderSystem->GetDepthStencilState();
	}

	//------------------------------------------------------------------
	IRenderPass* RenderManager::GetPass(PassType eType)
	{
		for (int i = 0; i > m_iPassCount; ++i)
		{
			if (m_pRenderPasses[i]->GetType() == eType)
				return m_pRenderPasses[i];
		}
		return NULL;
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

		HALgfx::IDevice* pDevice = ms_pRenderSystem->GetDevice();

#ifdef USE_COMMAND_BUFFER
		HALgfx::IDeviceContext* pDeviceContext = ms_pRenderSystem->GetDeferredDeviceContext();
#else
		HALgfx::IDeviceContext* pDeviceContext = ms_pRenderSystem->GetDeviceContext();
#endif

		HALgfx::ViewPort viewPort = ms_pRenderSystem->GetViewPort();
		HALgfx::IRenderTargetView* pRTV = ms_pRenderSystem->GetFrameBufferRTV();
		HALgfx::IRenderTargetView* pHDRRTV = ms_pRenderSystem->GetFrameBufferRTVHDR();
		HALgfx::IDepthStencilView* pDSV = ms_pRenderSystem->GetFrameBufferDSV();
		HALgfx::IDepthStencilState* pDSVStateEnabled = ms_pRenderSystem->GetDepthStencilState();
		HALgfx::IDepthStencilState* pDSVStateDisabled = ms_pRenderSystem->GetDepthStencilStateDisabled();
		HALgfx::IRasterizerState* pRState = ms_pRenderSystem->GetRasterizerState();

		// draw render passes
		for (int i = 0; i < m_iPassCount; ++i)
		{
			const PassType type = m_pRenderPasses[i]->GetType();

			if (type == PASS_DEPTH)
			{
				IRenderPass* pDepthPass = m_pRenderPasses[i];
				pDepthPass->SetRenderState(pDeviceContext, viewPort, NULL, pDSV, pRState, pDSVStateEnabled);
				pDepthPass->Render(pDevice, pDeviceContext);
				pDepthPass->ClearDrawNodes();
			}
			else if (type == PASS_SKY)
			{
				IRenderPass* pSkyboxPass = m_pRenderPasses[i];
				pSkyboxPass->SetRenderState(pDeviceContext, viewPort, pHDRRTV, pDSV, pRState, pDSVStateDisabled);
				pSkyboxPass->Render(pDevice, pDeviceContext);
				pSkyboxPass->ClearDrawNodes();
			}
			else if (type == PASS_SHADOW)
			{
				IRenderPass* pShadowPass = m_pRenderPasses[i];
				pShadowPass->SetRenderState(pDeviceContext, viewPort, 0, 0, pRState, pDSVStateEnabled);
				pShadowPass->Setup(pDevice, m_iWidth, m_iHeight);
				pShadowPass->Render(pDevice, pDeviceContext);
				pShadowPass->ClearDrawNodes();
			}
			else if (type == PASS_OPAQUE)
			{
				IRenderPass* pOpaquePass = m_pRenderPasses[i];
				pOpaquePass->SetRenderState(pDeviceContext, viewPort, pHDRRTV, pDSV, pRState, pDSVStateEnabled);
				pOpaquePass->Render(pDevice, pDeviceContext);
				pOpaquePass->ClearDrawNodes();
			}
			else if (type == PASS_POSTPROCESS)
			{
				IRenderPass* pPostprocessPass = m_pRenderPasses[i];
				pPostprocessPass->SetRenderState(pDeviceContext, viewPort, pRTV, pDSV, pRState, pDSVStateDisabled);
				pPostprocessPass->Setup(pDevice, m_iWidth, m_iHeight);
				pPostprocessPass->Render(pDevice, pDeviceContext);
			}
		}

		ms_pRenderSystem->Present();
	}

	void RenderManager::UpdateRenderResources()
	{
		HALgfx::IDevice* pDevice = ms_pRenderSystem->GetDevice();

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

						const Scene::IMaterial* pMaterial = pSurface->GetMaterial();
						if (pMaterial->GetType() == Scene::MATERIAL_SKY)
						{
							const Scene::MaterialSky* pMaterialSky = static_cast<const Scene::MaterialSky*>(pMaterial);
							GPUResourceManager::GetInstance().CreateCubeTextureResource(pMaterialSky->GetMapName(), pDevice);
						}

					}
				}
			}
		}
	}

//------------------------------------------------------------------
void RenderManager::UpdateRenderPasses()
{
	HALgfx::IDevice* pDevice = ms_pRenderSystem->GetDevice();
	for (int i = 0; i < m_iPassCount; ++i)
	{
		const PassType type = m_pRenderPasses[i]->GetType();

		if (type == PASS_DEPTH)
		{
			RenderPassDepth* pPass = static_cast<RenderPassDepth*>(m_pRenderPasses[i]);
			pPass->Setup(pDevice, m_iWidth, m_iHeight);
		}
		else if (type == PASS_SKY)
		{
			RenderPassSky* pPass = static_cast<RenderPassSky*>(m_pRenderPasses[i]);
			pPass->Setup(pDevice, m_iWidth, m_iHeight);
		}
		else if (type == PASS_SHADOW)
		{
			RenderPassShadow* pPass = static_cast<RenderPassShadow*>(m_pRenderPasses[i]);
			pPass->Setup(pDevice, m_iWidth, m_iHeight);
		}
		else if (type == PASS_OPAQUE)
		{
#ifdef USE_DIRECTX
			CopyShadowParameters();
#endif

			RenderPassOpaque* pPass = static_cast<RenderPassOpaque*>(m_pRenderPasses[i]);
			pPass->SetLightIntensityLevel(m_fLightIntensityLevel);
			pPass->Setup(pDevice, m_iWidth, m_iHeight);
		}
		else if (type == PASS_POSTPROCESS)
		{
			RenderPassPostprocess* pPass = static_cast<RenderPassPostprocess*>(m_pRenderPasses[i]);
			pPass->SetParams(m_iWidth, m_iHeight, m_fIntensityLevel);
			pPass->SetHDRSRV(GetFrameBufferSRVHDR());
			pPass->SetDepthSRV(GetDepthSRV());
			pPass->Setup(pDevice, m_iWidth, m_iHeight);
		}
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

void RenderManager::CopyShadowParameters()
{
	RenderPassShadow* pShadowPass = static_cast<RenderPassShadow*>(m_pRenderPasses[PASS_SHADOW]);
	RenderPassOpaque* pOpaquePass = static_cast<RenderPassOpaque*>(m_pRenderPasses[PASS_OPAQUE]);

	Math::Matrix4f mProjection[MAX_CASCADE_COUNT];
	Math::Matrix4f mView;
	HALgfx::IShaderResourceView* pShadowSRV = 0;
	Math::Vector4f v4Range;

	pShadowPass->GetShadowParameters(&mView, mProjection, &pShadowSRV, &v4Range);
	pOpaquePass->SetShadowParameters(&mView, mProjection, &pShadowSRV, &v4Range);
}

} // namespace Renderer
} // namespace Magnet