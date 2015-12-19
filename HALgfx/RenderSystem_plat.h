#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <d3d11.h>
#include "Viewport.h"

#include "IRenderTargetView.h"
#include "IShaderResourceView.h"

namespace Magnet
{
namespace HALgfx
{

class Device;
class IDevice;
class IDeviceContext;
class IRenderTargetView;
class IDepthStencilView;
class IDepthStencilState;
class IBlendState;
class IShader;
class ITexture2d;
class IRasterizerState;
class ISamplerState;

class RenderSystem
{

private:
	RenderSystem();
	~RenderSystem();
	RenderSystem(const RenderSystem&);
	RenderSystem& operator=(const RenderSystem&);

	static RenderSystem* ms_pInstance;

public:
	static void Initialize();
	static RenderSystem& GetInstance();
	static bool Exist();
	static void Terminate();

	bool InitializeSystem(unsigned int uWidth, unsigned int uHeight, void* pWindowHandle);
	void TerminateSystem();

	IDevice* GetDevice();
	IDeviceContext* GetDeviceContext();

	IShaderResourceView* GetFrameBufferSRVHDR();
	IRenderTargetView* GetFrameBufferRTV();
	IRenderTargetView* GetFrameBufferRTVHDR();
	IDepthStencilView* GetFrameBufferDSV();

	IRasterizerState* GetRasterizerState();
	ViewPort GetViewPort();
	IDepthStencilState* GetDepthStencilState();
	IDepthStencilState* GetDepthStencilStateDisabled();

	void GetSHFromCubemap(float faSHRed[9], float faSHGreen[9], float faSHBlue[9]) const;

	void Present();

private:
	IRenderTargetView* m_pFrameBufferRTV;
	
	ITexture2d* m_pTexture2dHDR;
	ITexture2d* m_pTexture2dDepthStencil;

	IRenderTargetView* m_pFrameBufferRTVHDR;
	IDepthStencilView* m_pFrameBufferDSV;
	IShaderResourceView* m_pFrameBufferSRVHDR;

	IDepthStencilState* m_pDepthStencilEnabled;
	IDepthStencilState* m_pDepthStencilDisabled;
	IRasterizerState* m_pRasterizerState;
	IBlendState* m_pColorWriteEnabled;
	IBlendState* m_pColorWriteDisabled;

	ViewPort m_viewPort;
	
	IDevice* m_pDevice;
	IDeviceContext* m_pImmediateDeviceContext;
	D3D_DRIVER_TYPE m_driverType;
	D3D_FEATURE_LEVEL m_featureLevel;
	IDXGISwapChain* m_pSwapChain;

	float m_faSHRed[9];
	float m_faSHGreen[9];
	float m_faSHBlue[9];
};

inline IDevice* RenderSystem::GetDevice()
{
	return m_pDevice;
}

inline IDeviceContext* RenderSystem::GetDeviceContext()
{
	return m_pImmediateDeviceContext;
}

inline IRenderTargetView* RenderSystem::GetFrameBufferRTV()
{
	return m_pFrameBufferRTV;
}

inline IRenderTargetView* RenderSystem::GetFrameBufferRTVHDR()
{
	return m_pFrameBufferRTVHDR;
}

inline IShaderResourceView* RenderSystem::GetFrameBufferSRVHDR()
{
	return m_pFrameBufferSRVHDR;
}

inline IDepthStencilView* RenderSystem::GetFrameBufferDSV()
{
	return m_pFrameBufferDSV;
}

inline ViewPort RenderSystem::GetViewPort()
{
	return m_viewPort;
}

inline IDepthStencilState* RenderSystem::GetDepthStencilState()
{
	return m_pDepthStencilEnabled;
}

inline IDepthStencilState* RenderSystem::GetDepthStencilStateDisabled()
{
	return m_pDepthStencilDisabled;
}

inline IRasterizerState* RenderSystem::GetRasterizerState()
{
	return m_pRasterizerState;
}

} // namespace HALgfx
} // namespace Magnet

#endif