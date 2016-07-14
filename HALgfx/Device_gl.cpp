
#include "Device_gl.h"

//#include "BlendState_plat.h"
//#include "DepthStencilState_plat.h"
//#include "RasterizerState_plat.h"
#include "SamplerState_gl.h"
//#include "DepthStencilView_plat.h"
//#include "RenderTargetView_plat.h"
#include "ShaderResourceView_gl.h"
//#include "UnorderedAccessView_plat.h"
#include "Buffer_gl.h"
#include "Texture2d_gl.h"
//#include "FeatureLevel.h"
#include "InputLayout_gl.h"
#include "PixelShader_gl.h"
#include "Program_gl.h"
#include "VertexShader_gl.h"
//#include "ComputeShader_gl.h"

namespace Magnet
{
namespace HALgfx
{
//------------------------------------------------------------------
FeatureLevel GLDevice::GetFeatureLevel()
{
	//return FEATURE_LEVEL_D3D11;
	return FEATURE_LEVEL_OPENGL;
}

//------------------------------------------------------------------
//IDeviceContext* Device::CreateContext()
//{
//
//}

//------------------------------------------------------------------
IBlendState* GLDevice::CreateBlendState(const BlendDesc& desc)
{
	return 0;
}

//------------------------------------------------------------------
IDepthStencilState* GLDevice::CreateDepthStencilState(const DepthStencilDesc& desc)
{
	return 0;
}

//------------------------------------------------------------------
IRasterizerState* GLDevice::CreateRasterizerState(const RasterizerDesc& desc)
{
	return 0;
}

//------------------------------------------------------------------
ISamplerState* GLDevice::CreateSamplerState(const SamplerStateDesc& desc)
{
	GLSamplerState* pSamplerState = new GLSamplerState(desc);
	return pSamplerState;
}

//------------------------------------------------------------------
IInputLayout* GLDevice::CreateInputLayout(int iNumElements, const InputElementDesc *pInputElementDescs, int iStride, int iShaderCodeSize, const void* pShaderCode)
{
	GLInputLayout* pInputLayout = new GLInputLayout(iNumElements, pInputElementDescs, iStride);
	return pInputLayout;
}

//------------------------------------------------------------------
IShader* GLDevice::CreateShader(ShaderType eType, int iShaderCodeSize, const void* pShaderCode)
{
	switch (eType)
	{
	case VERTEX_SHADER:
	{
		GLVertexShader* pVShader = new GLVertexShader();
		pVShader->Create(pShaderCode, iShaderCodeSize);
		return pVShader;
	}
	case PIXEL_SHADER:
	{
		GLPixelShader* pPShader = new GLPixelShader();
		pPShader->Create(pShaderCode, iShaderCodeSize);
		return pPShader;
	}
	case COMPUTE_SHADER:
	{
		return 0;
	}
	}
	return 0;
}

//------------------------------------------------------------------
ITexture2d* GLDevice::CreateTexture2d(const Texture2dDesc& desc, const SubResourceData* subResourceData)
{
	GLTexture2d* pTexture = new GLTexture2d();
	pTexture->Create(desc, subResourceData);
	return pTexture;
}

//------------------------------------------------------------------
IBuffer* GLDevice::CreateBuffer(const BufferDesc& desc, const SubResourceData& subResourceData)
{
	GLBuffer* pBuffer = new GLBuffer(desc);
	pBuffer->Create(desc, subResourceData);
	return pBuffer;
}

//------------------------------------------------------------------
IProgram* GLDevice::CreateProgram(const char* pName)
{
	IProgram* pProgram = new GLProgram(pName);
	return pProgram;
}

//------------------------------------------------------------------
IRenderTargetView* GLDevice::CreateRenderTargetView(IResource* pResource, const RenderTargetViewDesc& desc)
{
	return 0;
}

//------------------------------------------------------------------
IDepthStencilView* GLDevice::CreateDepthStencilView(IResource* pResource, const DepthStencilViewDesc& desc)
{
	return 0;
}

//------------------------------------------------------------------
IShaderResourceView* GLDevice::CreateShaderResourceView(IResource* pResource, const ShaderResourceViewDesc& desc)
{
	GLShaderResourceView* pSRV = new GLShaderResourceView();
	pSRV->Create(desc, pResource);
	return pSRV;
}

//------------------------------------------------------------------
IUnorderedAccessView* GLDevice::CreateUnorderedAccessView(IResource* pResource, const UnorderedAccessViewDesc& desc)
{
	return 0;
}

//------------------------------------------------------------------
void GLDevice::LoadCubeTextureResource(const char* pPath, ITexture2d** ppTexture, IShaderResourceView** ppSRV)
{
//	ID3D11Texture2D* pCubeTexture = NULL;
//	ID3D11ShaderResourceView* pCubeRV = NULL;



///	*ppTexture = new GLTexture2d(pCubeTexture);
//	*ppSRV = new GLShaderResourceView(pCubeRV);
}

//------------------------------------------------------------------
void GLDevice::SetupInputLayout(IInputLayout* pInputLayout, IBuffer* pVertexBuffer)
{
	GLInputLayout* pGLInputLayout = static_cast<GLInputLayout*>(pInputLayout);
	
	GLBuffer* pGLVertexBuffer = static_cast<GLBuffer*>(pVertexBuffer);

	pGLInputLayout->CreateVAO(pGLVertexBuffer->GetGLHandle());

}

} // namespace HALgfx
} // namespace Manget