#ifndef RENDER_SYSTEM_OVR_H
#define RENDER_SYSTEM_OVR_H

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

class RenderSystemOVR
{

//------------------------------------------------------------
// ovrSwapTextureSet wrapper class that also maintains the render target views
// needed for D3D11 rendering.
struct OculusTexture
{
	ovrHmd                   hmd;
	ovrSwapTextureSet      * TextureSet;
	static const int         TextureCount = 2;
	ID3D11RenderTargetView * TexRtv[TextureCount];

	OculusTexture() :
		hmd(nullptr),
		TextureSet(nullptr)
	{
		TexRtv[0] = TexRtv[1] = nullptr;
	}

	bool Init(ovrHmd _hmd, int sizeW, int sizeH)
	{
		hmd = _hmd;

		D3D11_TEXTURE2D_DESC dsDesc;
		dsDesc.Width = sizeW;
		dsDesc.Height = sizeH;
		dsDesc.MipLevels = 1;
		dsDesc.ArraySize = 1;
		dsDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		dsDesc.SampleDesc.Count = 1;   // No multi-sampling allowed
		dsDesc.SampleDesc.Quality = 0;
		dsDesc.Usage = D3D11_USAGE_DEFAULT;
		dsDesc.CPUAccessFlags = 0;
		dsDesc.MiscFlags = 0;
		dsDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

		ID3D11Device* pDevice = nullptr;

		ovrResult result = ovr_CreateSwapTextureSetD3D11(hmd, pDevice, &dsDesc, ovrSwapTextureSetD3D11_Typeless, &TextureSet);
		if (!OVR_SUCCESS(result))
			return false;

		assert(pDevice);

		assert(TextureSet->TextureCount == TextureCount);

		for (int i = 0; i < TextureCount; ++i)
		{
			ovrD3D11Texture* tex = (ovrD3D11Texture*)&TextureSet->Textures[i];
			D3D11_RENDER_TARGET_VIEW_DESC rtvd = {};
			rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			pDevice->CreateRenderTargetView(tex->D3D11.pTexture, &rtvd, &TexRtv[i]);
		}

		return true;
	}

	~OculusTexture()
	{
		for (int i = 0; i < TextureCount; ++i)
		{
			TexRtv[i]->Release();
		}
		if (TextureSet)
		{
			ovr_DestroySwapTextureSet(hmd, TextureSet);
		}
	}

	void AdvanceToNextTexture()
	{
		TextureSet->CurrentIndex = (TextureSet->CurrentIndex + 1) % TextureSet->TextureCount;
	}
};

struct DepthBuffer
{
	ID3D11DepthStencilView * TexDsv;

	DepthBuffer(ID3D11Device * Device, int sizeW, int sizeH, int sampleCount = 1)
	{
		DXGI_FORMAT format = DXGI_FORMAT_D32_FLOAT;
		D3D11_TEXTURE2D_DESC dsDesc;
		dsDesc.Width = sizeW;
		dsDesc.Height = sizeH;
		dsDesc.MipLevels = 1;
		dsDesc.ArraySize = 1;
		dsDesc.Format = format;
		dsDesc.SampleDesc.Count = sampleCount;
		dsDesc.SampleDesc.Quality = 0;
		dsDesc.Usage = D3D11_USAGE_DEFAULT;
		dsDesc.CPUAccessFlags = 0;
		dsDesc.MiscFlags = 0;
		dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		ID3D11Texture2D * Tex;
		Device->CreateTexture2D(&dsDesc, NULL, &Tex);
		Device->CreateDepthStencilView(Tex, NULL, &TexDsv);
	}
	~DepthBuffer()
	{
		TexDsv->Release();
	}
};

private:
	RenderSystemOVR();
	~RenderSystemOVR();
	RenderSystemOVR(const RenderSystemOVR&);
	RenderSystemOVR& operator=(const RenderSystemOVR&);

	static RenderSystemOVR* ms_pInstance;

public:
	static void Initialize();
	static RenderSystemOVR& GetInstance();
	static bool Exist();
	static void Terminate();

	bool InitializeSystem(bool retryCreate, unsigned int uWidth, unsigned int uHeight, void* pWindowHandle);
	void TerminateSystem();

	IDevice* GetDevice();
	IDeviceContext* GetDeviceContext();

	IShaderResourceView* GetFrameBufferSRVHDR();
	IShaderResourceView* GetDepthSRV();
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
	// OVR related
	ovrTexture     * m_pMirrorTexture;
	OculusTexture  * pEyeRenderTexture[2]; 
	DepthBuffer    * pEyeDepthBuffer[2];

	ovrHmd m_HMD;
	ovrGraphicsLuid m_luid;
	ovrHmdDesc m_hmdDesc;

	DepthBuffer* m_pMainDepthBuffer;
	ID3D11Texture2D        * m_pBackBufferTexture;
	ID3D11RenderTargetView * m_pBackBufferRTV;

	//
	IRenderTargetView* m_pFrameBufferRTV;

	ITexture2d* m_pTexture2dHDR;
	ITexture2d* m_pTexture2dDepthStencil;
	IShaderResourceView* m_pTexture2dDepthSRV;

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

inline IDevice* RenderSystemOVR::GetDevice()
{
	return m_pDevice;
}

inline IDeviceContext* RenderSystemOVR::GetDeviceContext()
{
	return m_pImmediateDeviceContext;
}

inline IRenderTargetView* RenderSystemOVR::GetFrameBufferRTV()
{
	return m_pFrameBufferRTV;
}

inline IRenderTargetView* RenderSystemOVR::GetFrameBufferRTVHDR()
{
	return m_pFrameBufferRTVHDR;
}

inline IShaderResourceView* RenderSystemOVR::GetFrameBufferSRVHDR()
{
	return m_pFrameBufferSRVHDR;
}

inline IShaderResourceView* RenderSystemOVR::GetDepthSRV()
{
	return m_pTexture2dDepthSRV;
}

inline IDepthStencilView* RenderSystemOVR::GetFrameBufferDSV()
{
	return m_pFrameBufferDSV;
}

inline ViewPort RenderSystemOVR::GetViewPort()
{
	return m_viewPort;
}

inline IDepthStencilState* RenderSystemOVR::GetDepthStencilState()
{
	return m_pDepthStencilEnabled;
}

inline IDepthStencilState* RenderSystemOVR::GetDepthStencilStateDisabled()
{
	return m_pDepthStencilDisabled;
}

inline IRasterizerState* RenderSystemOVR::GetRasterizerState()
{
	return m_pRasterizerState;
}

} // namespace HALgfx
} // namespace Magnet

#endif