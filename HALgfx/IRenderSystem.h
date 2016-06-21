#ifndef IRENDER_SYSTEM_H
#define IRENDER_SYSTEM_H

#include "Viewport.h"

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
class IShaderResourceView;
class ITexture2d;
class IRasterizerState;
class ISamplerState;


class IRenderSystem
{
public:

	virtual bool InitializeSystem(unsigned int uWidth, unsigned int uHeight, void* pWindowHandle) = 0;
	virtual void TerminateSystem() = 0;
	virtual void Present() = 0;

	IDevice* GetDevice();
	IDeviceContext* GetDeviceContext();
	IDeviceContext* GetDeferredDeviceContext();

	IShaderResourceView* GetFrameBufferSRVHDR();
	IShaderResourceView* GetDepthSRV();
	IRenderTargetView* GetFrameBufferRTV();
	IRenderTargetView* GetFrameBufferRTVHDR();
	IDepthStencilView* GetFrameBufferDSV();

	IRasterizerState* GetRasterizerState();
	ViewPort GetViewPort();
	IDepthStencilState* GetDepthStencilState();
	IDepthStencilState* GetDepthStencilStateDisabled();

protected:
	// frame buffer
	IRenderTargetView* m_pFrameBufferRTV;

	// depth
	ITexture2d* m_pTexture2dDepthStencil;
	IDepthStencilView* m_pFrameBufferDSV;
	IShaderResourceView* m_pTexture2dDepthSRV;

	// hdr frame buffer
	ITexture2d* m_pTexture2dHDR;
	IRenderTargetView* m_pFrameBufferRTVHDR;
	IShaderResourceView* m_pFrameBufferSRVHDR;

	// depth stencil state
	IDepthStencilState* m_pDepthStencilEnabled;
	IDepthStencilState* m_pDepthStencilDisabled;

	// rasterization state
	IRasterizerState* m_pRasterizerState;
	IBlendState* m_pColorWriteEnabled;
	IBlendState* m_pColorWriteDisabled;

	ViewPort m_viewPort;

	IDevice* m_pDevice;
	IDeviceContext* m_pImmediateDeviceContext;
	IDeviceContext* m_pDeferredDeviceContext;
};

inline IDevice* IRenderSystem::GetDevice()
{
	return m_pDevice;
}

inline IDeviceContext* IRenderSystem::GetDeviceContext()
{
	return m_pImmediateDeviceContext;
}

inline IDeviceContext* IRenderSystem::GetDeferredDeviceContext()
{
	return m_pDeferredDeviceContext;
}

inline IRenderTargetView* IRenderSystem::GetFrameBufferRTV()
{
	return m_pFrameBufferRTV;
}

inline IRenderTargetView* IRenderSystem::GetFrameBufferRTVHDR()
{
	return m_pFrameBufferRTVHDR;
}

inline IShaderResourceView* IRenderSystem::GetFrameBufferSRVHDR()
{
	return m_pFrameBufferSRVHDR;
}

inline IShaderResourceView* IRenderSystem::GetDepthSRV()
{
	return m_pTexture2dDepthSRV;
}

inline IDepthStencilView* IRenderSystem::GetFrameBufferDSV()
{
	return m_pFrameBufferDSV;
}

inline ViewPort IRenderSystem::GetViewPort()
{
	return m_viewPort;
}

inline IDepthStencilState* IRenderSystem::GetDepthStencilState()
{
	return m_pDepthStencilEnabled;
}

inline IDepthStencilState* IRenderSystem::GetDepthStencilStateDisabled()
{
	return m_pDepthStencilDisabled;
}

inline IRasterizerState* IRenderSystem::GetRasterizerState()
{
	return m_pRasterizerState;
}

} // namespace HALgfx
} // namespace Magnet



#endif