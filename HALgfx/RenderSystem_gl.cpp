
#include <assert.h>
#include <stdio.h>

#include "glew.h"		// has to be included before gl.h
#include <gl/gl.h>
#include <gl/glu.h>

#include "BlendState_gl.h"
#include "CommandBuffer_gl.h"
#include "Device_gl.h"
#include "DeviceContext_gl.h"
#include "DepthStencilView_gl.h"
#include "DepthStencilState_gl.h"
#include "IDevice.h"
#include "ITexture2d.h"
#include "RenderSystem_gl.h"
#include "RenderTargetView_gl.h"
#include "RasterizerState_gl.h"
#include "ShaderResourceView_gl.h"
#include "SamplerState_gl.h"
#include "Texture2d_gl.h"

namespace Magnet
{
namespace HALgfx
{

//------------------------------------------------------------------
GLRenderSystem* GLRenderSystem::ms_pInstance = 0;

//------------------------------------------------------------------
GLRenderSystem::GLRenderSystem()
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

}

//------------------------------------------------------------------
GLRenderSystem::~GLRenderSystem()
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
}

//------------------------------------------------------------------
void GLRenderSystem::Initialize()
{
	assert(ms_pInstance == 0);
	ms_pInstance = new GLRenderSystem();
}

//------------------------------------------------------------------
GLRenderSystem* GLRenderSystem::GetInstance()
{
	assert(ms_pInstance != 0);
	return ms_pInstance;
}

//------------------------------------------------------------------
bool GLRenderSystem::Exist()
{
	return ms_pInstance != 0;
}

//------------------------------------------------------------------
void GLRenderSystem::Terminate()
{
	delete ms_pInstance;
	ms_pInstance = 0;
}

//------------------------------------------------------------------
bool GLRenderSystem::InitializeSystem(unsigned int uWidth, unsigned int uHeight, void* pWindowHandle)
{
	static PIXELFORMATDESCRIPTOR pfd =	// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),	// Size Of This Pixel Format Descriptor
		1,								// Version Number
		PFD_DRAW_TO_WINDOW |			// Format Must Support Window
		PFD_SUPPORT_OPENGL |			// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,				// Must Support Double Buffering
		PFD_TYPE_RGBA,					// Request An RGBA Format
		8,								// Select Our Color Depth
		0, 0, 0, 0, 0, 0,				// Color Bits Ignored
		0,								// No Alpha Buffer
		0,								// Shift Bit Ignored
		0,								// No Accumulation Buffer
		0, 0, 0, 0,						// Accumulation Bits Ignored
		16,								// 16Bit Z-Buffer (Depth Buffer)
		0,								// No Stencil Buffer
		0,								// No Auxiliary Buffer
		PFD_MAIN_PLANE,					// Main Drawing Layer
		0,								// Reserved
		0, 0, 0							// Layer Masks Ignored
	};

	HWND hWnd = static_cast<HWND>(pWindowHandle);
	m_hDC = GetDC(hWnd);
	int pixelFormat = ChoosePixelFormat(m_hDC, &pfd);
	SetPixelFormat(m_hDC, pixelFormat, &pfd);
	HGLRC hRC = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, hRC);

	ShowWindow(static_cast<HWND>(pWindowHandle), SW_SHOW);		// Show The Window
	SetForegroundWindow(hWnd);									// Slightly Higher Priority
	SetFocus(hWnd);												// Sets Keyboard Focus To The Window
	
	glFrontFace(GL_CW);		// set counter clockwise polygon indices so it can use the same index data as DirectX
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	GLDevice* pDevice = new GLDevice();
	m_pDevice = static_cast<IDevice*>(pDevice);

	GLDeviceContext* pDeviceContext = new GLDeviceContext(hRC);
	m_pImmediateDeviceContext = static_cast<IDeviceContext*>(pDeviceContext);

	GLint glewInitResult = glewInit();
	if (GLEW_OK != glewInitResult)
	{
		printf("ERROR: %s\n", glewGetErrorString(glewInitResult));
		exit(EXIT_FAILURE);
	}

	glEnable(GL_DEBUG_OUTPUT);

	// final render target
	//m_pFrameBufferRTV = pDevice->CreateRenderTargetView();

	// view port
	m_viewPort.width = uWidth;
	m_viewPort.height = uHeight;
	m_viewPort.topLeftX = 0;
	m_viewPort.topLeftY = 0;
	m_viewPort.minDepth = 0.0f;
	m_viewPort.maxDepth = 1.f;

#if 0
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

	// depth SRV
	ShaderResourceViewDesc srvDescDepth;
	srvDescDepth.format = FORMAT_R24_UNORM_X8_TYPELESS;
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
	rDesc.depthClipEnable = true;
	rDesc.slopeScaleDepthBias = 0.f;
	rDesc.scissorEnable = false;
	rDesc.depthBiasClamp = 0.f;
	rDesc.fillMode = FILL_SOLID;
	rDesc.antialiasedLineEnable = true;
	rDesc.multisampleEnable = true;
	m_pRasterizerState = pDevice->CreateRasterizerState(rDesc);

#endif

	return true;
}

//------------------------------------------------------------------
void GLRenderSystem::Present()
{
#ifdef USE_COMMAND_BUFFER
	HALgfx::ICommandBuffer* pCommandBuffer = m_pDeferredDeviceContext->FinishCommandBuffer();
	m_pImmediateDeviceContext->ExecuteCommandBuffer(pCommandBuffer, false);
#endif

	SwapBuffers(m_hDC);
}

//------------------------------------------------------------------
void GLRenderSystem::TerminateSystem()
{

}

} // namespace HALgfx
} // namespace Magnet
