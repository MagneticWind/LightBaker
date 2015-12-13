//#include <d3d11_1.h>
#include <assert.h>
//#include <D3DX11tex.h>
#include <stdio.h>

#include "BlendState_plat.h"
#include "Device_plat.h"
#include "DeviceContext_plat.h"
#include "DepthStencilView_plat.h"
#include "DepthStencilState_plat.h"
#include "IDevice.h"
#include "ITexture2d.h"
#include "RenderSystem_plat.h"
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
RenderSystem* RenderSystem::ms_pInstance = 0;

//------------------------------------------------------------------
RenderSystem::RenderSystem()
{
	m_pFrameBufferRTV = 0;
	m_pTexture2dHDR = 0;
	m_pTexture2dDepthStencil = 0;
	m_pFrameBufferRTVHDR = 0;
	m_pFrameBufferDSV = 0;

	m_pDepthStencilEnabled = 0;
	m_pDepthStencilDisabled = 0;
	m_pRasterizerState = 0;
	m_pColorWriteEnabled = 0;
	m_pColorWriteDisabled = 0;

	m_pImmediateDeviceContext = 0;

	m_pSwapChain = 0;
}

//------------------------------------------------------------------
RenderSystem::~RenderSystem()
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

	if (m_pFrameBufferRTVHDR)
	{
		delete m_pFrameBufferRTVHDR;
		m_pFrameBufferRTVHDR = 0;
	}

	if( m_pFrameBufferDSV)
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
void RenderSystem::Initialize()
{
	assert(ms_pInstance == 0);
	ms_pInstance = new RenderSystem();
}

//------------------------------------------------------------------
RenderSystem& RenderSystem::GetInstance()
{
	assert(ms_pInstance != 0);
	return *ms_pInstance;
}

//------------------------------------------------------------------
bool RenderSystem::Exist()
{
	return ms_pInstance != 0;
}

//------------------------------------------------------------------
void RenderSystem::Terminate()
{
	delete ms_pInstance;
	ms_pInstance = 0;
}

//------------------------------------------------------------------
bool RenderSystem::InitializeSystem(unsigned int uWidth, unsigned int uHeight, void* pWindowHandle)
{
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
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof(sd) );
	sd.BufferCount = 1;
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
	hr = D3D11CreateDeviceAndSwapChain( NULL, m_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
		D3D11_SDK_VERSION, &sd, &m_pSwapChain, &pD3DDevice, &m_featureLevel, &pImmediateContext );

	if( FAILED( hr ) )
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
	texture2dDSTDesc.format = FORMAT_R24G8_TYPELESS;
	texture2dDSTDesc.usage = USAGE_DEFAULT;
	texture2dDSTDesc.bindFlags = BIND_SHADER_RESOURCE | BIND_DEPTH_STENCIL;
	m_pTexture2dDepthStencil = pDevice->CreateTexture2d(texture2dDSTDesc, subResourceData);

	// depth stencil target
	DepthStencilViewDesc dstDesc;
	dstDesc.format = FORMAT_D24_UNORM_S8_UINT;
	dstDesc.viewDimension = DSV_DIMENSION_TEXTURE2D;
	dstDesc.texture2d.mipSlice = 0;
	m_pFrameBufferDSV = pDevice->CreateDepthStencilView(m_pTexture2dDepthStencil, dstDesc);

	// depth stencil state
	DepthStencilDesc sdDesc;
	sdDesc.depthEnable = true;
	sdDesc.depthFunc = COMPARISON_LESS_EQUAL;
	sdDesc.depthWriteMask = DEPTH_WRITE_MASK_ZERO;
	sdDesc.stencilEnable = true;
	sdDesc.frontFace.stencilFunc = COMPARISON_ALWAYS;
	sdDesc.frontFace.stencilPassOp = STENCIL_OP_KEEP;
	sdDesc.frontFace.stencilDepthFailOp = STENCIL_OP_INCR;
	sdDesc.frontFace.stencilFailOp = STENCIL_OP_KEEP;
	sdDesc.backFace.stencilFunc = COMPARISON_ALWAYS;
	sdDesc.backFace.stencilPassOp = STENCIL_OP_KEEP;
	sdDesc.backFace.stencilDepthFailOp = STENCIL_OP_INCR;
	sdDesc.backFace.stencilFailOp = STENCIL_OP_KEEP;
	m_pDepthStencilEnabled = pDevice->CreateDepthStencilState(sdDesc);

	sdDesc.depthEnable = false;
	sdDesc.stencilEnable = false;
	sdDesc.depthWriteMask = DEPTH_WRITE_MASK_ALL;

	m_pDepthStencilDisabled = pDevice->CreateDepthStencilState(sdDesc);

	// rasterizer state
	RasterizerDesc rDesc;
	rDesc.cullMode = CULL_FRONT;
	rDesc.depthBias = 0;
	rDesc.depthClipEnable = true;
	rDesc.scissorEnable = false;
	rDesc.depthBiasClamp = 0.f;
	rDesc.fillMode = FILL_SOLID;
	rDesc.antialiasedLineEnable = false;
	rDesc.multisampleEnable = false;
	m_pRasterizerState = pDevice->CreateRasterizerState(rDesc);

	// initialize perf\debug event wrapper
	//ID3DUserDefinedAnnotation* pPerf;
	//HRESULT hr = pImmediateContext->QueryInterface( __uuidof(pPerf), reinterpret_cast<void**>(&pPerf) );

	return true;
}

//------------------------------------------------------------------
void RenderSystem::GetSHFromCubemap(float faSHRed[9], float faSHGreen[9], float faSHBlue[9]) const
{
	for(int i = 0; i < 9; ++i)
	{
		faSHRed[i] = m_faSHRed[i];
		faSHGreen[i] = m_faSHGreen[i];
		faSHBlue[i] = m_faSHBlue[i];
	}
}

//------------------------------------------------------------------
//void RenderSystem::LoadCubeTextureResource(const char* pPath, ITexture2d** ppTexture, IShaderResourceView** ppSRV)
//{
//	D3DX11_IMAGE_LOAD_INFO LoadInfo;
//	LoadInfo.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//	ID3D11Texture2D* pCubeTexture = NULL;
//	ID3D11ShaderResourceView* pCubeRV = NULL;
//	Device* pDevice = static_cast<Device*>(m_pDevice);
//	D3DX11CreateShaderResourceViewFromFile( pDevice->GetD3DPtr(), pPath, &LoadInfo, NULL, &pCubeRV, NULL );
//
//	pCubeRV->GetResource( ( ID3D11Resource** )&pCubeTexture );
//
//	*ppTexture = new Texture2d(pCubeTexture);
//	*ppSRV = new ShaderResourceView(pCubeRV);
//
//	DeviceContext* pDeviceContext = static_cast<DeviceContext*>(m_pImmediateDeviceContext);
//	D3DX11SHProjectCubeMap(pDeviceContext->GetD3DPtr(), 3, pCubeTexture, m_faSHRed, m_faSHGreen, m_faSHBlue);
//}

//------------------------------------------------------------------
void RenderSystem::Present()
{
	m_pSwapChain->Present(0, 0);
}

//------------------------------------------------------------------
void RenderSystem::TerminateSystem()
{
	
}

} // namespace HALgfx
} // namespace Magnet
