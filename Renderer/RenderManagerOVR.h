#ifndef RENDER_MANAGER_OVR_H
#define RENDER_MANAGER_OVR_H

#include "DrawNode.h"

namespace Magnet
{

namespace HALgfx
{
class IDevice;
class IDeviceContext;
class IRenderTargetView;
class IDepthStencilView;
class IRasterizerState;
class IDepthStencilState;
class ISamplerState;
struct ViewPort;
}

namespace Scene
{
class Texture;
class Mesh;
}

namespace Renderer
{
class RenderPassOpaque;
class RenderPassSky;
enum PassType;
class Shader;

typedef void(*CallBackLoadShader) (Shader* pShader);
typedef void(*CallBackLoadMesh) (Scene::Mesh* pMesh);

class RenderManagerOVR
{
private:
	RenderManagerOVR();
	~RenderManagerOVR();
	RenderManagerOVR(const RenderManagerOVR&);
	RenderManagerOVR& operator=(const RenderManagerOVR&);

	static RenderManagerOVR* ms_pInstance;

public:
	static RenderManagerOVR& GetInstance();
	static void Initialize(int iWidth, int iHeight, void* pWindowHandle);
	static bool Exist();
	static void Terminate();

public:

	// HALgfx::RenderSystem related
	HALgfx::IDevice* GetDevice();
	HALgfx::IDeviceContext* GetDeviceContext();
	HALgfx::IRenderTargetView* GetFrameBufferRTV();
	HALgfx::IRenderTargetView* GetFrameBufferRTVHDR();
	HALgfx::IShaderResourceView* GetFrameBufferSRVHDR();
	HALgfx::IShaderResourceView* GetDepthSRV();
	HALgfx::IDepthStencilView* GetFrameBufferDSV();
	HALgfx::IRasterizerState* GetRasterizerState();
	HALgfx::ViewPort GetViewPort();
	HALgfx::IDepthStencilState* GetDepthStencilState();
	HALgfx::ISamplerState* GetLinearSamplerState();
	HALgfx::ISamplerState* GetAnisotropicSamplerState();

	void RenderOneFrame();
	void UpdateRenderResources();
	void UpdateRenderPasses();
	void SetupRenderPasses();
	void DestroyPasses();

	void SetFrameBufferDimension(int iWidth, int iHeight);
	void GetFrameBufferDimension(int& iWidth, int& iHeight) const;

	void SetMajorLightData(const Math::Vector3f& v3Direction, const Math::Vector3f& v3Color);
	void GetMajorLightData(Math::Vector3f& v3Direction, Math::Vector3f& v3Color) const;

	void SetLoadShaderCallBack(CallBackLoadShader callBackLoadShader);
	void SetLoadMeshCallBack(CallBackLoadMesh callBackLoadMesh);

	//	void LoadTextureResource(const char* pPath, Scene::Texture* pTexture);

	void GetSHCubemap(Math::Vector4f shCoeffs[], int iNumCoeffs);

private:

	void InitializeHALSystem(int iWidth, int iHeight);

	RenderPassOpaque* m_pOpaquePass;
	RenderPassSky* m_pSkyPass;

	CallBackLoadShader m_callBackLoadShader;
	CallBackLoadMesh m_callBackLoadMesh;

	// 
	int m_iWidth;
	int m_iHeight;

	//@TODO: design some structure for lights if support multiple lights
	Math::Vector3f m_v3MajorLightDirection;
	Math::Vector3f m_v3MajorLightColor;

	int iLights;

};
} // namespace Renderer
} // namespace Magnet

#endif