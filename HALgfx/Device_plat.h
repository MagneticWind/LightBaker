#ifndef DEVICE_H
#define DEVICE_H

#include <d3d11.h>
#include "IDevice.h"

namespace Magnet
{
namespace HALgfx
{

class Device : public IDevice
{
public:
	Device(ID3D11Device* pDevice) : m_pDevice(pDevice)
	{
	}
	virtual FeatureLevel GetFeatureLevel();
	//virtual IDeviceContext* CreateContext();

	virtual IBlendState* CreateBlendState(const BlendDesc&);
	virtual IDepthStencilState* CreateDepthStencilState(const DepthStencilDesc&);
	virtual IRasterizerState* CreateRasterizerState(const RasterizerDesc&);
	virtual ISamplerState* CreateSamplerState(const SamplerStateDesc&);

	virtual IInputLayout* CreateInputLayout(int iNumElements, const InputElementDesc *pInputElementDescs, int iShaderCodeSize, const void* pShaderCode);
	virtual IShader* CreateShader(ShaderType eType, int iShaderCodeSize, const void* pShaderCode);
	virtual ITexture2d* CreateTexture2d(const Texture2dDesc&, const SubResourceData&);
	virtual IBuffer* CreateBuffer(const BufferDesc&, const SubResourceData&);

	virtual IRenderTargetView* CreateRenderTargetView(IResource*, const RenderTargetViewDesc&);
	virtual IDepthStencilView* CreateDepthStencilView(IResource*, const DepthStencilViewDesc&);
	virtual IShaderResourceView* CreateShaderResourceView(IResource*, const ShaderResourceViewDesc&);
	//virtual IUnorderedAccessView* CreateUnorderedAccessView(IResource*);

	ID3D11Device* GetD3DPtr();

	IRenderTargetView* CreateRenderTargetView(IDXGISwapChain*);

private:
	ID3D11Device* m_pDevice;
};

} // namespace HALgfx
} // namespace Magnet

#endif