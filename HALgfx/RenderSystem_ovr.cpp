//#include <d3d11_1.h>
#include <assert.h>
#include <stdio.h>

#include "OVR_CAPI_D3D.h"

#include "BlendState_plat.h"
#include "Device_plat.h"
#include "DeviceContext_plat.h"
#include "DepthStencilView_plat.h"
#include "DepthStencilState_plat.h"
#include "IDevice.h"
#include "ITexture2d.h"
#include "RenderSystem_ovr.h"
#include "RenderTargetView_plat.h"
#include "RasterizerState_plat.h"
#include "ShaderResourceView_plat.h"
#include "SamplerState_plat.h"
#include "Texture2d_plat.h"

namespace Magnet
{
namespace HALgfx
{

//------------------------------------------------------------------
RenderSystemOVR* RenderSystemOVR::ms_pInstance = 0;

//------------------------------------------------------------------
RenderSystemOVR::RenderSystemOVR()
{
	m_pFrameBufferRTV = 0;
	m_pTexture2dHDR = 0;
	m_pTexture2dDepthStencil = 0;
	m_pFrameBufferRTVHDR = 0;
	m_pFrameBufferDSV = 0;

	m_pTexture2dDepthSRV = 0;

	m_pDepthStencilEnabled = 0;
	m_pDepthStencilDisabled = 0;
	m_pRasterizerState = 0;
	m_pColorWriteEnabled = 0;
	m_pColorWriteDisabled = 0;

	m_pImmediateDeviceContext = 0;

	m_pSwapChain = 0;
}

//------------------------------------------------------------------
RenderSystemOVR::~RenderSystemOVR()
{
	if (m_pFrameBufferRTV)
	{
		delete m_pFrameBufferRTV;
		m_pFrameBufferRTV = 0;
	}

	if (m_pTexture2dHDR)
	{
		delete m_pTexture2dHDR;
		m_pTexture2dHDR = 0;
	}

	if (m_pTexture2dDepthStencil)
	{
		delete m_pTexture2dDepthStencil;
		m_pTexture2dDepthStencil = 0;
	}

	if (m_pTexture2dDepthSRV)
	{
		delete m_pTexture2dDepthSRV;
		m_pTexture2dDepthSRV = 0;
	}

	if (m_pFrameBufferRTVHDR)
	{
		delete m_pFrameBufferRTVHDR;
		m_pFrameBufferRTVHDR = 0;
	}

	if (m_pFrameBufferDSV)
	{
		delete m_pFrameBufferDSV;
		m_pFrameBufferDSV = 0;
	}

	if (m_pDepthStencilEnabled)
	{
		delete m_pDepthStencilEnabled;
		m_pDepthStencilEnabled = 0;
	}

	if (m_pDepthStencilDisabled)
	{
		delete m_pDepthStencilDisabled;
		m_pDepthStencilDisabled = 0;
	}

	if (m_pRasterizerState)
	{
		delete m_pRasterizerState;
		m_pRasterizerState = 0;
	}

	if (m_pColorWriteEnabled)
	{
		delete m_pColorWriteEnabled;
		m_pColorWriteEnabled = 0;
	}

	if (m_pColorWriteDisabled)
	{
		delete m_pColorWriteDisabled;
		m_pColorWriteDisabled = 0;
	}

	if (m_pDevice)
	{
		delete m_pDevice;
		m_pDevice = 0;
	}

	if (m_pImmediateDeviceContext)
	{
		delete m_pImmediateDeviceContext;
		m_pImmediateDeviceContext = 0;
	}

	if (m_pSwapChain)
	{
		m_pSwapChain->Release();
	}
}

//------------------------------------------------------------------
void RenderSystemOVR::Initialize()
{
	assert(ms_pInstance == 0);
	ms_pInstance = new RenderSystemOVR();
}

//------------------------------------------------------------------
RenderSystemOVR& RenderSystemOVR::GetInstance()
{
	assert(ms_pInstance != 0);
	return *ms_pInstance;
}

//------------------------------------------------------------------
bool RenderSystemOVR::Exist()
{
	return ms_pInstance != 0;
}

//------------------------------------------------------------------
void RenderSystemOVR::Terminate()
{
	delete ms_pInstance;
	ms_pInstance = 0;
}

//------------------------------------------------------------------
bool RenderSystemOVR::InitializeSystem(bool retryCreate, unsigned int uWidth, unsigned int uHeight, void* pWindowHandle)
{

	ovrResult result = ovr_Create(&m_HMD, &m_luid);
	if (!OVR_SUCCESS(result))
		return retryCreate;

	ovrHmdDesc hmdDesc = ovr_GetHmdDesc(m_HMD);

	/*RECT size = { 0, 0, uWidth, uHeight };
	AdjustWindowRect(&size, WS_OVERLAPPEDWINDOW, false);
	const UINT flags = SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW;
	if (!SetWindowPos(pWindowHandle, nullptr, 0, 0, size.right - size.left, size.bottom - size.top, flags))
		return false;

	IDXGIFactory * DXGIFactory = nullptr;
	HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)(&DXGIFactory));
	VALIDATE((hr == ERROR_SUCCESS), "CreateDXGIFactory1 failed");

	IDXGIAdapter * Adapter = nullptr;
	for (UINT iAdapter = 0; DXGIFactory->EnumAdapters(iAdapter, &Adapter) != DXGI_ERROR_NOT_FOUND; ++iAdapter)
	{
		DXGI_ADAPTER_DESC adapterDesc;
		Adapter->GetDesc(&adapterDesc);
		if ((pLuid == nullptr) || memcmp(&adapterDesc.AdapterLuid, pLuid, sizeof(LUID)) == 0)
			break;
		Release(Adapter);
	}

	auto DriverType = Adapter ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_HARDWARE;
	hr = D3D11CreateDevice(Adapter, DriverType, 0, 0, 0, 0, D3D11_SDK_VERSION, &Device, 0, &Context);
	Release(Adapter);
	VALIDATE((hr == ERROR_SUCCESS), "D3D11CreateDevice failed");*/

	HRESULT hr = S_OK;

	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = uWidth;
	sd.BufferDesc.Height = uHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
	sd.OutputWindow = static_cast<HWND>(pWindowHandle);
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	ID3D11Device* pD3DDevice = 0;
	ID3D11DeviceContext* pImmediateContext = 0;

	m_driverType = D3D_DRIVER_TYPE_HARDWARE;
	hr = D3D11CreateDeviceAndSwapChain(NULL, m_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
		D3D11_SDK_VERSION, &sd, &m_pSwapChain, &pD3DDevice, &m_featureLevel, &pImmediateContext);

	if (FAILED(hr))
	{
		printf("Failed to create DirectX 11 system.");
		assert(0);
	}

	Device* pDevice = new Device(pD3DDevice);
	m_pDevice = static_cast<IDevice*>(pDevice);

	DeviceContext* pDeviceContext = new DeviceContext(pImmediateContext);
	m_pImmediateDeviceContext = static_cast<IDeviceContext*>(pDeviceContext);

	// final render target
	m_pFrameBufferRTV = pDevice->CreateRenderTargetView(m_pSwapChain);

	// ovr
	// Create backbuffer
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pBackBufferTexture);
	hr = pD3DDevice->CreateRenderTargetView(m_pBackBufferTexture, NULL, &m_pBackBufferRTV);
	assert(hr == ERROR_SUCCESS);

	// Main depth buffer
	m_pMainDepthBuffer = new DepthBuffer(pD3DDevice, uWidth, uHeight);
	//pDeviceContext->OMSetRenderTargets(1, &BackBufferRT, MainDepthBuffer->TexDsv);

	// Make the eye render buffers (caution if actual size < requested due to HW limits). 
	ovrRecti         eyeRenderViewport[2];

	for (int eye = 0; eye < 2; ++eye)
	{
		ovrSizei idealSize = ovr_GetFovTextureSize(m_HMD, (ovrEyeType)eye, hmdDesc.DefaultEyeFov[eye], 1.0f);
		pEyeRenderTexture[eye] = new OculusTexture();
		if (!pEyeRenderTexture[eye]->Init(m_HMD, idealSize.w, idealSize.h))
		{
			//if (retryCreate) goto Done;
			assert(OVR_SUCCESS(result));
		}
		pEyeDepthBuffer[eye] = new DepthBuffer(pD3DDevice, idealSize.w, idealSize.h);
		eyeRenderViewport[eye].Pos.x = 0;
		eyeRenderViewport[eye].Pos.y = 0;
		eyeRenderViewport[eye].Size = idealSize;
		if (!pEyeRenderTexture[eye]->TextureSet)
		{
			//if (retryCreate) goto Done;
			assert(false);
		}
	}

	// Create a mirror to see on the monitor.
	D3D11_TEXTURE2D_DESC td;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	td.Width = uWidth;
	td.Height = uHeight;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.SampleDesc.Count = 1;
	td.MipLevels = 1;
	result = ovr_CreateMirrorTextureD3D11(m_HMD, pD3DDevice, &td, 0, &m_pMirrorTexture);
	if (!OVR_SUCCESS(result))
	{
		//if (retryCreate) goto Done;
		assert(false);
	}

	// Setup VR components, filling out description
	ovrEyeRenderDesc eyeRenderDesc[2];
	eyeRenderDesc[0] = ovr_GetRenderDesc(m_HMD, ovrEye_Left, hmdDesc.DefaultEyeFov[0]);
	eyeRenderDesc[1] = ovr_GetRenderDesc(m_HMD, ovrEye_Right, hmdDesc.DefaultEyeFov[1]);

#if 0
	// view port
	m_viewPort.width = uWidth;
	m_viewPort.height = uHeight;
	m_viewPort.topLeftX = 0;
	m_viewPort.topLeftY = 0;
	m_viewPort.minDepth = 0.f;
	m_viewPort.maxDepth = 1.f;

	// HDR render target texture
	Texture2dDesc texture2dDesc;
	texture2dDesc.width = uWidth;
	texture2dDesc.height = uHeight;
	texture2dDesc.format = FORMAT_R16G16B16A16_FLOAT;
	texture2dDesc.usage = USAGE_DEFAULT;
	texture2dDesc.bindFlags = BIND_SHADER_RESOURCE | BIND_RENDER_TARGET;

	SubResourceData subResourceData;
	subResourceData.pMem = 0;
	m_pTexture2dHDR = pDevice->CreateTexture2d(texture2dDesc, subResourceData);

	// HDR SRV
	ShaderResourceViewDesc srvDesc;
	srvDesc.format = texture2dDesc.format;
	srvDesc.viewDimension = SRV_DIMENSION_TEXTURE2D;
	srvDesc.texSRV.mipLevels = 1;
	srvDesc.texSRV.mostDetailedMip = 0;

	m_pFrameBufferSRVHDR = pDevice->CreateShaderResourceView(m_pTexture2dHDR, srvDesc);

	// HDR render target
	RenderTargetViewDesc rtvDesc;
	rtvDesc.format = texture2dDesc.format;
	rtvDesc.viewDimension = RTV_DIMENSION_TEXTURE2D;
	m_pFrameBufferRTVHDR = pDevice->CreateRenderTargetView(m_pTexture2dHDR, rtvDesc);

	// depth stencil target texture
	Texture2dDesc texture2dDSTDesc;
	texture2dDSTDesc.width = uWidth;
	texture2dDSTDesc.height = uHeight;
	texture2dDSTDesc.format = FORMAT_R32_TYPELESS;
	texture2dDSTDesc.usage = USAGE_DEFAULT;
	texture2dDSTDesc.bindFlags = BIND_SHADER_RESOURCE | BIND_DEPTH_STENCIL;
	m_pTexture2dDepthStencil = pDevice->CreateTexture2d(texture2dDSTDesc, subResourceData);

	// depth stencil target
	DepthStencilViewDesc dstDesc;
	dstDesc.format = FORMAT_D32_FLOAT;
	dstDesc.viewDimension = DSV_DIMENSION_TEXTURE2D;
	dstDesc.texture2d.mipSlice = 0;
	m_pFrameBufferDSV = pDevice->CreateDepthStencilView(m_pTexture2dDepthStencil, dstDesc);

	// depth SRV
	ShaderResourceViewDesc srvDescDepth;
	srvDescDepth.format = FORMAT_R32_FLOAT;
	srvDescDepth.viewDimension = SRV_DIMENSION_TEXTURE2D;
	srvDescDepth.texSRV.mipLevels = 1;
	srvDescDepth.texSRV.mostDetailedMip = 0;
	m_pTexture2dDepthSRV = pDevice->CreateShaderResourceView(m_pTexture2dDepthStencil, srvDescDepth);

	// depth stencil state
	DepthStencilDesc sdDesc;
	sdDesc.depthEnable = true;
	sdDesc.depthFunc = COMPARISON_LESS_EQUAL;
	sdDesc.depthWriteMask = DEPTH_WRITE_MASK_ALL;
	sdDesc.stencilEnable = false;
	sdDesc.frontFace.stencilFunc = COMPARISON_ALWAYS;
	sdDesc.frontFace.stencilPassOp = STENCIL_OP_KEEP;
	sdDesc.frontFace.stencilDepthFailOp = STENCIL_OP_KEEP;
	sdDesc.frontFace.stencilFailOp = STENCIL_OP_KEEP;
	sdDesc.backFace = sdDesc.frontFace;
	m_pDepthStencilEnabled = pDevice->CreateDepthStencilState(sdDesc);

	sdDesc.depthEnable = false;
	sdDesc.stencilEnable = false;
	sdDesc.depthWriteMask = DEPTH_WRITE_MASK_ALL;

	m_pDepthStencilDisabled = pDevice->CreateDepthStencilState(sdDesc);

	// rasterizer state
	RasterizerDesc rDesc;
	rDesc.cullMode = CULL_BACK;
	rDesc.frontCounterClockwise = false;
	rDesc.depthBias = 0;
	rDesc.depthClipEnable = false;
	rDesc.scissorEnable = false;
	rDesc.depthBiasClamp = 0.f;
	rDesc.fillMode = FILL_SOLID;
	rDesc.antialiasedLineEnable = true;
	rDesc.multisampleEnable = true;
	m_pRasterizerState = pDevice->CreateRasterizerState(rDesc);

	// initialize perf\debug event wrapper
	//ID3DUserDefinedAnnotation* pPerf;
	//HRESULT hr = pImmediateContext->QueryInterface( __uuidof(pPerf), reinterpret_cast<void**>(&pPerf) );
#endif

	return true;
}

//------------------------------------------------------------------
void RenderSystemOVR::GetSHFromCubemap(float faSHRed[9], float faSHGreen[9], float faSHBlue[9]) const
{
	for (int i = 0; i < 9; ++i)
	{
		faSHRed[i] = m_faSHRed[i];
		faSHGreen[i] = m_faSHGreen[i];
		faSHBlue[i] = m_faSHBlue[i];
	}
}

//------------------------------------------------------------------
void RenderSystemOVR::Present()
{
	m_pSwapChain->Present(0, 0);
}

//------------------------------------------------------------------
void RenderSystemOVR::TerminateSystem()
{

}

} // namespace HALgfx
} // namespace Magnet
