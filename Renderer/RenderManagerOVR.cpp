#include <assert.h>

#include "OVR_CAPI_D3D.h"

#include "GPUResourceManager.h"

#include "HALgfx\RenderSystem_ovr.h"
#include "IRenderPass.h"

#include "RenderManagerOVR.h"
#include "RenderPassOpaque.h"
#include "RenderPassSky.h"

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
RenderManagerOVR* RenderManagerOVR::ms_pInstance = 0;


//------------------------------------------------------------------
RenderManagerOVR::~RenderManagerOVR()
{

}

//------------------------------------------------------------------
RenderManagerOVR& RenderManagerOVR::GetInstance()
{
	assert(ms_pInstance != 0);
	return *ms_pInstance;
}

//------------------------------------------------------------------
void RenderManagerOVR::Initialize(int iWidth, int iHeight, void* pWindowHandle)
{
	// initializa HALRenderSystem
	HALgfx::RenderSystemOVR::Initialize();
	HALgfx::RenderSystemOVR::GetInstance().InitializeSystem(true, iWidth, iHeight, pWindowHandle);

	GPUResourceManager::Initialize();

	assert(ms_pInstance == 0);

	ms_pInstance = new RenderManagerOVR();

	ms_pInstance->SetupRenderPasses();

	ms_pInstance->SetFrameBufferDimension(iWidth, iHeight);

}

//------------------------------------------------------------------
void RenderManagerOVR::SetupRenderPasses()
{
	m_pOpaquePass = new RenderPassOpaque();
	m_pSkyPass = new RenderPassSky();
}

//------------------------------------------------------------------
bool RenderManagerOVR::Exist()
{
	return ms_pInstance != 0;
}

//------------------------------------------------------------------
void RenderManagerOVR::Terminate()
{
	delete ms_pInstance;
	ms_pInstance = 0;

	GPUResourceManager::Terminate();
	HALgfx::RenderSystemOVR::Terminate();
}

//------------------------------------------------------------------
HALgfx::IDevice* RenderManagerOVR::GetDevice()
{
	return HALgfx::RenderSystemOVR::GetInstance().GetDevice();
}

//------------------------------------------------------------------
HALgfx::IDeviceContext* RenderManagerOVR::GetDeviceContext()
{
	return HALgfx::RenderSystemOVR::GetInstance().GetDeviceContext();
}

//------------------------------------------------------------------
HALgfx::IRenderTargetView* RenderManagerOVR::GetFrameBufferRTV()
{
	return HALgfx::RenderSystemOVR::GetInstance().GetFrameBufferRTV();
}

//------------------------------------------------------------------
HALgfx::IRenderTargetView* RenderManagerOVR::GetFrameBufferRTVHDR()
{
	return HALgfx::RenderSystemOVR::GetInstance().GetFrameBufferRTVHDR();
}

//------------------------------------------------------------------
HALgfx::IShaderResourceView* RenderManagerOVR::GetFrameBufferSRVHDR()
{
	return HALgfx::RenderSystemOVR::GetInstance().GetFrameBufferSRVHDR();
}

//------------------------------------------------------------------
HALgfx::IShaderResourceView* RenderManagerOVR::GetDepthSRV()
{
	return HALgfx::RenderSystemOVR::GetInstance().GetDepthSRV();
}

//------------------------------------------------------------------
HALgfx::IDepthStencilView* RenderManagerOVR::GetFrameBufferDSV()
{
	return HALgfx::RenderSystemOVR::GetInstance().GetFrameBufferDSV();
}

//------------------------------------------------------------------
HALgfx::IRasterizerState* RenderManagerOVR::GetRasterizerState()
{
	return HALgfx::RenderSystemOVR::GetInstance().GetRasterizerState();
}

//------------------------------------------------------------------
HALgfx::ViewPort RenderManagerOVR::GetViewPort()
{
	return HALgfx::RenderSystemOVR::GetInstance().GetViewPort();
}

//------------------------------------------------------------------
HALgfx::IDepthStencilState* RenderManagerOVR::GetDepthStencilState()
{
	return HALgfx::RenderSystemOVR::GetInstance().GetDepthStencilState();
}

//------------------------------------------------------------------
void RenderManagerOVR::SetFrameBufferDimension(int iWidth, int iHeight)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;
}

//------------------------------------------------------------------
void RenderManagerOVR::GetFrameBufferDimension(int& iWidth, int& iHeight) const
{
	iWidth = m_iWidth;
	iHeight = m_iHeight;
}

//------------------------------------------------------------------
void RenderManagerOVR::RenderOneFrame()
{
	// create gpu resources for meshes, textures
	UpdateRenderResources();

	// create shader related resources, setup shader nodes, draw nodes
	UpdateRenderPasses();

	HALgfx::RenderSystemOVR& renderSystem = HALgfx::RenderSystemOVR::GetInstance();
	HALgfx::IDevice* pDevice = renderSystem.GetDevice();
	HALgfx::IDeviceContext* pDeviceContext = renderSystem.GetDeviceContext();

	HALgfx::ViewPort viewPort = renderSystem.GetViewPort();
	HALgfx::IRenderTargetView* pRTV = renderSystem.GetFrameBufferRTV();
	HALgfx::IRenderTargetView* pHDRRTV = renderSystem.GetFrameBufferRTVHDR();
	HALgfx::IDepthStencilView* pDSV = renderSystem.GetFrameBufferDSV();
	HALgfx::IDepthStencilState* pDSVStateEnabled = renderSystem.GetDepthStencilState();
	HALgfx::IDepthStencilState* pDSVStateDisabled = renderSystem.GetDepthStencilStateDisabled();
	HALgfx::IRasterizerState* pRState = renderSystem.GetRasterizerState();

	// Get both eye poses simultaneously, with IPD offset already included. 
	ovrPosef         EyeRenderPose[2];
	ovrVector3f      HmdToEyeViewOffset[2] = { eyeRenderDesc[0].HmdToEyeViewOffset,
		eyeRenderDesc[1].HmdToEyeViewOffset };
	double frameTime = ovr_GetPredictedDisplayTime(HMD, 0);
	// Keeping sensorSampleTime as close to ovr_GetTrackingState as possible - fed into the layer
	double           sensorSampleTime = ovr_GetTimeInSeconds();
	ovrTrackingState hmdState = ovr_GetTrackingState(HMD, frameTime, ovrTrue);
	ovr_CalcEyePoses(hmdState.HeadPose.ThePose, HmdToEyeViewOffset, EyeRenderPose);

	// Render Scene to Eye Buffers
	for (int eye = 0; eye < 2; ++eye)
	{
		m_pSkyPass->SetRenderState(pDeviceContext, viewPort, pHDRRTV, pDSV, pRState, pDSVStateEnabled);
		m_pSkyPass->Render(pDevice, pDeviceContext);
		m_pSkyPass->ClearDrawNodes();

		m_pOpaquePass->SetRenderState(pDeviceContext, viewPort, pHDRRTV, pDSV, pRState, pDSVStateEnabled);
		m_pOpaquePass->Render(pDevice, pDeviceContext);
		m_pOpaquePass->ClearDrawNodes();
	}

	// Initialize our single full screen Fov layer.
	ovrLayerEyeFov ld = {};
	ld.Header.Type = ovrLayerType_EyeFov;
	ld.Header.Flags = 0;

	for (int eye = 0; eye < 2; ++eye)
	{
		ld.ColorTexture[eye] = pEyeRenderTexture[eye]->TextureSet;
		ld.Viewport[eye] = eyeRenderViewport[eye];
		ld.Fov[eye] = hmdDesc.DefaultEyeFov[eye];
		ld.RenderPose[eye] = EyeRenderPose[eye];
		ld.SensorSampleTime = sensorSampleTime;
	}

	ovrLayerHeader* layers = &ld.Header;
	result = ovr_SubmitFrame(HMD, 0, nullptr, &layers, 1);

	renderSystem.Present();
}

void RenderManagerOVR::UpdateRenderResources()
{
	HALgfx::RenderSystemOVR& renderSystem = HALgfx::RenderSystemOVR::GetInstance();
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
void RenderManagerOVR::UpdateRenderPasses()
{
	HALgfx::RenderSystemOVR& renderSystem = HALgfx::RenderSystemOVR::GetInstance();
	HALgfx::IDevice* pDevice = renderSystem.GetDevice();
	for (int i = 0; i < PASS_NUMBER; ++i)
	{
		else if (i == PASS_OPAQUE)
		{
			RenderPassOpaque* pPass = static_cast<RenderPassOpaque*>(m_pRenderPasses[i]);
			pPass->SetLightIntensityLevel(m_fLightIntensityLevel);
		}
		m_pRenderPasses[i]->Setup(pDevice);
	}
}

//------------------------------------------------------------------
void RenderManagerOVR::SetMajorLightData(const Math::Vector3f& v3Direction, const Math::Vector3f& v3Color)
{
	m_v3MajorLightDirection = v3Direction;
	m_v3MajorLightColor = v3Color;
}

//------------------------------------------------------------------
void RenderManagerOVR::GetMajorLightData(Math::Vector3f& v3Direction, Math::Vector3f& v3Color) const
{
	v3Direction = m_v3MajorLightDirection;
	v3Color = m_v3MajorLightColor;
}

//------------------------------------------------------------------
void RenderManagerOVR::SetLoadShaderCallBack(CallBackLoadShader callBackLoadShader)
{
	m_callBackLoadShader = callBackLoadShader;
}

//------------------------------------------------------------------
void RenderManagerOVR::SetLoadMeshCallBack(CallBackLoadMesh callBackLoadMesh)
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
void RenderManagerOVR::GetSHCubemap(Math::Vector4f shCoeffs[], int iNumCoeffs)
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