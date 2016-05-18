
#include <assert.h>

#include "CBufferDefs.h"
#include "GPUResourceManager.h"

#include "HALgfx\IBuffer.h"
#include "HALgfx\IDevice.h"
#include "HALgfx\IDeviceContext.h"
#include "HALgfx\ISamplerState.h"
#include "HALgfx\IShaderResourceView.h"
#include "HALgfx\IDepthStencilView.h"
#include "HALgfx\ITexture2d.h"

#include "Scene\IRenderObject.h"
#include "Scene\RenderObject.h"
#include "Scene\SceneManager.h"
#include "Scene\Scene.h"
#include "Scene\Surface.h"
#include "Scene\Light.h"

#include "Math\AABB.h"

#include "RenderPassShadow.h"
#include "ShaderNode.h"

namespace Magnet
{
namespace Renderer
{

namespace RenderPassShadowPrivate
{
	static const float CASCADE_DISTANCES[MAX_CASCADE_COUNT + 1] = { 1, 8, 15, 20, 50 };
	
	//debug
	//static const float CASCADE_DISTANCES[MAX_CASCADE_COUNT + 1] = { 0.5, 20, 30, 40, 50 };
}

//------------------------------------------------------------------
RenderPassShadow::RenderPassShadow() : m_pTexture2dShadowmap(0), m_pShadowmapDSV(0), m_pShadowmapSRV(0), m_bInitialized(false)
{
}

//------------------------------------------------------------------
RenderPassShadow::~RenderPassShadow()
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
void RenderPassShadow::SetRenderState(HALgfx::IDeviceContext* pDeviceContext, const HALgfx::ViewPort& viewPort,
	HALgfx::IRenderTargetView* pRTV, HALgfx::IDepthStencilView* pDSV, HALgfx::IRasterizerState* pRState, HALgfx::IDepthStencilState* pDSState)
{
	// ignore viewport setup
	// pDeviceContext->SetViewport(viewport);

	if (pRTV == NULL)
	{
		HALgfx::IRenderTargetView* paRTVs[1] = { NULL };
		pDeviceContext->SetRenderTargetViews(1, paRTVs, m_pShadowmapDSV);
	}
	else
	{
		pDeviceContext->SetRenderTargetViews(1, &pRTV, m_pShadowmapDSV);
	}

	pDeviceContext->SetRasterizerState(pRState);
	pDeviceContext->SetDepthStencilState(pDSState);

	pDeviceContext->ClearDepthStencilView(m_pShadowmapDSV, HALgfx::CLEAR_DEPTH, 1.f, 0);

	pDeviceContext->SetPrimitiveTopology(HALgfx::PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

//------------------------------------------------------------------
void RenderPassShadow::Render(HALgfx::IDevice* pDevice, HALgfx::IDeviceContext* pDeviceContext)
{
	pDeviceContext->BeginEvent("RenderPassShadow");
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
PassType RenderPassShadow::GetType()
{
	return PASS_SHADOW;
}

//------------------------------------------------------------------
void RenderPassShadow::ClearDrawNodes()
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
void RenderPassShadow::Setup(HALgfx::IDevice* pDevice, int iWidth, int iHeight)
{

	if (!m_bInitialized)
	{
		Initialize(pDevice, iWidth, iHeight);
		m_bInitialized = true;
	}

	Scene::Scene* pScene = Scene::SceneManager::GetInstance().GetScene();
	std::list<Scene::IRenderObject*>&renderObjectList = pScene->GetRenderObjectList();

	// camera
	const Scene::Camera* pCamera = pScene->GetCurrentCamera();
	const Math::Matrix4f& mView = pCamera->GetViewMatrix();
	const Math::Matrix4f& mProjection = pCamera->GetProjectionMatrix();

	// directional light that casts shadow
	std::list<Scene::Light*>& lightList = pScene->GetLightList();
	Scene::Light* pMainLight = *lightList.begin();
	assert(pMainLight->GetType() == Scene::LIGHT_DIRECTIONAL);
	const Math::Vector3f& v3LightDirection = pMainLight->GetDirection();
	const Math::Vector3f v3LightPosition = pMainLight->GetPosition();
	const Math::Vector3f v3Up(0.f, 1.f, 0.f);

	Math::Vector3f v3Direction = -v3LightDirection;
	v3Direction.Normalize();

	// shadow view matrix
	Math::MatrixLookAtLH(v3LightPosition, v3LightPosition + v3Direction, v3Up, m_mShadowView);

	const Math::Vector3f& v3Position = pCamera->GetPosition();
	const Math::Vector3f& v3ViewDir = pCamera->GetDirection();
	const Math::Vector3f& v3UpDir = pCamera->GetUpVector();
	const float fFov = pCamera->GetFOV();
	const float fAspectRatio = pCamera->GetAspectRatio();

	Math::Vector3f v3Left = Math::Cross3f(v3UpDir, v3ViewDir);
	v3Left.Normalize();

	Math::Vector3f v3YDirection = Math::Cross3f(v3ViewDir, v3Left);
	v3YDirection.Normalize();

	for (int i = 0; i < MAX_CASCADE_COUNT; ++i)
	{
		m_frustum.Update(v3Position, v3ViewDir, v3Left, v3YDirection, RenderPassShadowPrivate::CASCADE_DISTANCES[i], RenderPassShadowPrivate::CASCADE_DISTANCES[i + 1], fFov, fAspectRatio);

		const Math::Vector3f* const pCornerPoints = m_frustum.GetCornerPoints();

		// transform into the shadow view space, compute an AABB
		Math::AABB bbox;
		for (int j = 0; j < 8; ++j)
		{
			Math::Vector3f transformedPosition = m_mShadowView * pCornerPoints[j];

			bbox.Update(transformedPosition);
		}
		const Math::Vector3f& v3Min = bbox.GetMinPoint();
		const Math::Vector3f& v3Max = bbox.GetMaxPoint();

		// project matrix for the cascade
		Math::MatrixOrthographicOffCenterLH(v3Min.x, v3Max.x, v3Min.y, v3Max.y, v3Min.z, v3Max.z, m_mProjection[i]);

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

						if (pMaterial->GetTech() & Scene::SHADOW_CASTER)
						{
							char shaderName[256];
							strcpy(shaderName, "csm_");

							const char* meshName = pSurface->GetGeometry()->GetName();
							GPUResourceManager& gpuResourceManager = GPUResourceManager::GetInstance();
							MeshResource& meshResource = gpuResourceManager.GetMeshResource(std::string(meshName));

							strcat(shaderName, meshResource.m_caInputLayoutString);

							ShaderNode* pShaderNode = NULL;
							ShaderNodeExists(shaderName, m_lShaderNodes, &pShaderNode);

							// add shader node
							if (pShaderNode == NULL)
							{
								pShaderNode = new ShaderNode(shaderName);
								pShaderNode->LoadShader(HALgfx::VERTEX_SHADER);
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

								drawNode.m_bSetViewport = true;
								drawNode.mViewport = m_viewPorts[i];

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
								pTransform->mView = m_mShadowView;
								pTransform->mProjection = m_mProjection[i];

								pShaderNode->AddDrawNode(drawNode);
							}

						}
					}
				}
			}
		}
	}

}

void RenderPassShadow::Initialize(HALgfx::IDevice* pDevice, int iWidth, int iHeight)
{
	HALgfx::SubResourceData data;

	// shadowmap texture
	HALgfx::Texture2dDesc texture2dShadowmapDesc;
	texture2dShadowmapDesc.width = 1024;
	texture2dShadowmapDesc.height = texture2dShadowmapDesc.width * MAX_CASCADE_COUNT;
	texture2dShadowmapDesc.format = HALgfx::FORMAT_R32_TYPELESS;
	texture2dShadowmapDesc.usage = HALgfx::USAGE_DEFAULT;
	texture2dShadowmapDesc.bindFlags = HALgfx::BIND_SHADER_RESOURCE | HALgfx::BIND_DEPTH_STENCIL;
	m_pTexture2dShadowmap = pDevice->CreateTexture2d(texture2dShadowmapDesc, data);

	// shadowmap depth stencil target
	HALgfx::DepthStencilViewDesc shadowmapDstDesc;
	shadowmapDstDesc.format = HALgfx::FORMAT_D32_FLOAT;
	shadowmapDstDesc.viewDimension = HALgfx::DSV_DIMENSION_TEXTURE2D;
	shadowmapDstDesc.texture2d.mipSlice = 0;
	m_pShadowmapDSV = pDevice->CreateDepthStencilView(m_pTexture2dShadowmap, shadowmapDstDesc);

	// shadowmap SRV
	HALgfx::ShaderResourceViewDesc shadowmapSrvDesc;
	shadowmapSrvDesc.format = HALgfx::FORMAT_R32_FLOAT;
	shadowmapSrvDesc.viewDimension = HALgfx::SRV_DIMENSION_TEXTURE2D;
	shadowmapSrvDesc.texSRV.mipLevels = 1;
	shadowmapSrvDesc.texSRV.mostDetailedMip = 0;
	m_pShadowmapSRV = pDevice->CreateShaderResourceView(m_pTexture2dShadowmap, shadowmapSrvDesc);

	// viewports
	for (int i = 0; i < MAX_CASCADE_COUNT; ++i)
	{
		m_viewPorts[i].width = texture2dShadowmapDesc.width;
		m_viewPorts[i].height = texture2dShadowmapDesc.width;
		m_viewPorts[i].topLeftX = 0;
		m_viewPorts[i].topLeftY = texture2dShadowmapDesc.width * i;
		m_viewPorts[i].minDepth = 0.f;
		m_viewPorts[i].maxDepth = 1.f;
	}

	m_bInitialized = true;
}

void RenderPassShadow::GetShadowParameters(Math::Matrix4f* mShadowView, Math::Matrix4f* mProjection, HALgfx::IShaderResourceView** pShadowmapSRV, Math::Vector4f* pRange)
{
	*mShadowView = m_mShadowView;

	for (int i = 0; i < MAX_CASCADE_COUNT; ++i)
	{
		mProjection[i] = m_mProjection[i];
	}
	*pRange = Math::Vector4f(RenderPassShadowPrivate::CASCADE_DISTANCES[1], RenderPassShadowPrivate::CASCADE_DISTANCES[2],
		RenderPassShadowPrivate::CASCADE_DISTANCES[3], RenderPassShadowPrivate::CASCADE_DISTANCES[4]);
	*pShadowmapSRV = m_pShadowmapSRV;
}


} // namespace Renderer
} // namespace Magnet