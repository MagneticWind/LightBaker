
#include "Device_gl.h"

//#include "BlendState_plat.h"
//#include "DepthStencilState_plat.h"
//#include "RasterizerState_plat.h"
//#include "SamplerState_plat.h"
//#include "DepthStencilView_plat.h"
//#include "RenderTargetView_plat.h"
//#include "ShaderResourceView_plat.h"
//#include "UnorderedAccessView_plat.h"
//#include "Buffer_plat.h"
//#include "Texture2d_plat.h"
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
	return 0;
}

//------------------------------------------------------------------
IInputLayout* GLDevice::CreateInputLayout(int iNumElements, const InputElementDesc *pInputElementDescs, int iShaderCodeSize, const void* pShaderCode)
{
	return 0;
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
		return 0;
	}
	case COMPUTE_SHADER:
	{
		return 0;
	}
	}
	return 0;
}

//------------------------------------------------------------------
ITexture2d* GLDevice::CreateTexture2d(const Texture2dDesc& desc, const SubResourceData& subResourceData)
{
	return 0;
}

//------------------------------------------------------------------
IBuffer* GLDevice::CreateBuffer(const BufferDesc& desc, const SubResourceData& subResourceData)
{
	return 0;
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
	return 0;
}

//------------------------------------------------------------------
IUnorderedAccessView* GLDevice::CreateUnorderedAccessView(IResource* pResource, const UnorderedAccessViewDesc& desc)
{
	return 0;
}

//------------------------------------------------------------------
void GLDevice::LoadCubeTextureResource(const char* pPath, ITexture2d** ppTexture, IShaderResourceView** ppSRV)
{

}

} // namespace HALgfx
} // namespace Manget