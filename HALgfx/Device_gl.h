#ifndef GL_DEVICE_H
#define GL_DEVICE_H

#include "glew.h"		// has to be included before gl.h
#include <gl/gl.h>
#include <gl/glu.h>

#include "IDevice.h"

namespace Magnet
{
namespace HALgfx
{

class GLDevice : public IDevice
{
public:
	GLDevice()
	{
	}
	virtual FeatureLevel GetFeatureLevel();
	//virtual IDeviceContext* CreateContext();

	virtual IBlendState* CreateBlendState(const BlendDesc&);
	virtual IDepthStencilState* CreateDepthStencilState(const DepthStencilDesc&);
	virtual IRasterizerState* CreateRasterizerState(const RasterizerDesc&);
	virtual ISamplerState* CreateSamplerState(const SamplerStateDesc&);

	virtual IInputLayout* CreateInputLayout(int iNumElements, const InputElementDesc *pInputElementDescs, int iStride, int iShaderCodeSize, const void* pShaderCode);
	virtual IShader* CreateShader(ShaderType eType, int iShaderCodeSize, const void* pShaderCode);
	virtual ITexture2d* CreateTexture2d(const Texture2dDesc&, const SubResourceData*);
	virtual IBuffer* CreateBuffer(const BufferDesc&, const SubResourceData&);
	virtual IProgram* CreateProgram(const char* pName);

	virtual IRenderTargetView* CreateRenderTargetView(IResource*, const RenderTargetViewDesc&);
	virtual IDepthStencilView* CreateDepthStencilView(IResource*, const DepthStencilViewDesc&);
	virtual IShaderResourceView* CreateShaderResourceView(IResource*, const ShaderResourceViewDesc&);
	virtual IUnorderedAccessView* CreateUnorderedAccessView(IResource*, const UnorderedAccessViewDesc&);

	virtual void LoadCubeTextureResource(const char* pPath, ITexture2d** ppTexture, IShaderResourceView** ppSRV);

	virtual void SetupInputLayout(IInputLayout* pInputLayout, IBuffer* pVertexBuffer);

private:

};

} // namespace HALgfx
} // namespace Magnet

#endif