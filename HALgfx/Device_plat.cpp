#include <D3DX11tex.h>

#include "Device_plat.h"
#include "BlendState_plat.h"
#include "DepthStencilState_plat.h"
#include "RasterizerState_plat.h"
#include "SamplerState_plat.h"
#include "DepthStencilView_plat.h"
#include "RenderTargetView_plat.h"
#include "ShaderResourceView_plat.h"
#include "UnorderedAccessView_plat.h"
#include "Buffer_plat.h"
#include "Texture2d_plat.h"
#include "FeatureLevel.h"
#include "InputLayout_plat.h"
#include "PixelShader_plat.h"
#include "Program_plat.h"
#include "VertexShader_plat.h"
#include "ComputeShader_plat.h"

namespace Magnet
{
namespace HALgfx
{
//------------------------------------------------------------------
FeatureLevel Device::GetFeatureLevel()
{
	return FEATURE_LEVEL_D3D11;
}

//------------------------------------------------------------------
//IDeviceContext* Device::CreateContext()
//{
//
//}

//------------------------------------------------------------------
IBlendState* Device::CreateBlendState(const BlendDesc& desc)
{
	BlendState* pBlendState = new BlendState(desc);
	pBlendState->Create(m_pDevice);
	return pBlendState;
}

//------------------------------------------------------------------
IDepthStencilState* Device::CreateDepthStencilState(const DepthStencilDesc& desc)
{
	DepthStencilState* pDepthStencilState = new DepthStencilState(desc);
	pDepthStencilState->Create(m_pDevice);
	return pDepthStencilState;
}

//------------------------------------------------------------------
IRasterizerState* Device::CreateRasterizerState(const RasterizerDesc& desc)
{
	RasterizerState* pRasterizerState = new RasterizerState();
	pRasterizerState->Create(desc, m_pDevice);
	return pRasterizerState;
}

//------------------------------------------------------------------
ISamplerState* Device::CreateSamplerState(const SamplerStateDesc& desc)
{
	SamplerState* pSamplerState = new SamplerState(desc);
	pSamplerState->Create(m_pDevice);
	return pSamplerState;
}

//------------------------------------------------------------------
IInputLayout* Device::CreateInputLayout(int iNumElements, const InputElementDesc *pInputElementDescs, int iShaderCodeSize, const void* pShaderCode)
{
	InputLayout* pInputLayout = new InputLayout(iNumElements, pInputElementDescs, iShaderCodeSize, pShaderCode);
	pInputLayout->Create(m_pDevice);
	return pInputLayout;
}

//------------------------------------------------------------------
IShader* Device::CreateShader(ShaderType eType, int iShaderCodeSize, const void* pShaderCode)
{
	switch(eType)
	{
	case VERTEX_SHADER:
		{
			VertexShader* pVShader = new VertexShader();
			pVShader->Create(pShaderCode,iShaderCodeSize, m_pDevice);
			return pVShader;
		}
	case PIXEL_SHADER:
		{
			PixelShader* pPShader = new PixelShader();
			pPShader->Create(pShaderCode, iShaderCodeSize, m_pDevice);
			return pPShader;
		}
	case COMPUTE_SHADER:
		{
			ComputeShader* pCShader = new ComputeShader();
			pCShader->Create(pShaderCode, iShaderCodeSize, m_pDevice);
			return pCShader;
		}
	}
	return 0;
}

//------------------------------------------------------------------
IProgram* Device::CreateProgram(const char* pName)
{
	IProgram* pProgram = new Program(pName);
	return pProgram;
}

//------------------------------------------------------------------
ITexture2d* Device::CreateTexture2d(const Texture2dDesc& desc, const SubResourceData& subResourceData)
{
	Texture2d* pTexture2d = new Texture2d();
	pTexture2d->Create(desc, subResourceData, m_pDevice);
	return pTexture2d;
}

//------------------------------------------------------------------
IBuffer* Device::CreateBuffer(const BufferDesc& desc, const SubResourceData& subResourceData)
{
	Buffer* pBuffer = new Buffer(desc);
	pBuffer->Create(m_pDevice, desc, subResourceData);
	return pBuffer;
}

//------------------------------------------------------------------
IRenderTargetView* Device::CreateRenderTargetView(IResource* pResource, const RenderTargetViewDesc& desc)
{
	RenderTargetView* pRenderTargetView = new RenderTargetView();
	pRenderTargetView->Create(desc, pResource, m_pDevice);
	return pRenderTargetView;
}

//------------------------------------------------------------------
ID3D11Device* Device::GetD3DPtr()
{
	return m_pDevice;
}

//------------------------------------------------------------------
IRenderTargetView* Device::CreateRenderTargetView(IDXGISwapChain* pSwapChain)
{
	RenderTargetView* pRenderTargetView = new RenderTargetView();
	pRenderTargetView->Create(pSwapChain, m_pDevice);
	return pRenderTargetView;
}

//------------------------------------------------------------------
IDepthStencilView* Device::CreateDepthStencilView(IResource* pResource, const DepthStencilViewDesc& desc)
{
	DepthStencilView* pDepthStencilView = new DepthStencilView();
	pDepthStencilView->Create(desc, pResource, m_pDevice);
	return pDepthStencilView;
}

//------------------------------------------------------------------
IShaderResourceView* Device::CreateShaderResourceView(IResource* pResource, const ShaderResourceViewDesc& desc)
{
	ShaderResourceView* pShaderResourceView = new ShaderResourceView();
	pShaderResourceView->Create(desc, pResource, m_pDevice);
	return pShaderResourceView;
}

//------------------------------------------------------------------
IUnorderedAccessView* Device::CreateUnorderedAccessView(IResource* pResource, const UnorderedAccessViewDesc& desc)
{
	UnorderedAccessView* pUnorderedAccessView = new UnorderedAccessView();
	pUnorderedAccessView->Create(desc, pResource, m_pDevice);
	return pUnorderedAccessView;
}

//------------------------------------------------------------------
void Device::LoadCubeTextureResource(const char* pPath, ITexture2d** ppTexture, IShaderResourceView** ppSRV)
{
	D3DX11_IMAGE_LOAD_INFO LoadInfo;
	LoadInfo.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	ID3D11Texture2D* pCubeTexture = NULL;
	ID3D11ShaderResourceView* pCubeRV = NULL;
	D3DX11CreateShaderResourceViewFromFile(m_pDevice, pPath, &LoadInfo, NULL, &pCubeRV, NULL);

	pCubeRV->GetResource((ID3D11Resource**)&pCubeTexture);

	*ppTexture = new Texture2d(pCubeTexture);
	*ppSRV = new ShaderResourceView(pCubeRV);
}

} // namespace HALgfx
} // namespace Manget