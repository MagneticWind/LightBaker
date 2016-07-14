#ifndef IDEVICE_H
#define IDEVICE_H

#include "FeatureLevel.h"
#include "ShaderType.h"

namespace Magnet
{
namespace HALgfx
{
class IDeviceContext;
class IBlendState;
class IDepthStencilState;
class IRasterizerState;
class ISamplerState;
class IScissorState;
class IInputLayout;
class IProgram;
class IShader;
class IBuffer;
class ITexture2d;

class IRenderTargetView;
class IDepthStencilView;
class IShaderResourceView;
class IUnorderedAccessView;

struct DepthStencilDesc;
struct RasterizerDesc;
struct SamplerStateDesc;
struct ScissorStateDesc;
struct RenderTargetViewDesc;
struct DepthStencilViewDesc;
struct ShaderResourceViewDesc;
struct UnorderedAccessViewDesc;
struct ViewPort;
struct BlendDesc;

struct InputLayoutDesc;
struct Texture2dDesc;
struct SubResourceData;
struct BufferDesc;
struct InputElementDesc;

class IVertexShader;

class IResource;

class IDevice
{
public:
	virtual ~IDevice(){}
	virtual FeatureLevel GetFeatureLevel() = 0;
	//virtual IDeviceContext* CreateContext();

	virtual IBlendState* CreateBlendState(const BlendDesc&) = 0;
	virtual IDepthStencilState* CreateDepthStencilState(const DepthStencilDesc&) = 0;
	virtual IRasterizerState* CreateRasterizerState(const RasterizerDesc&) = 0;
	virtual ISamplerState* CreateSamplerState(const SamplerStateDesc&) = 0;

	virtual IInputLayout* CreateInputLayout(int iNumElements, const InputElementDesc *pInputElementDescs, int iStride, int iShaderCodeSize, const void* pShaderCode) = 0;
	virtual IShader* CreateShader(ShaderType eType, int iShaderCodeSize, const void* pShaderCode) = 0;
	virtual IProgram* CreateProgram(const char* pName) = 0;
	virtual ITexture2d* CreateTexture2d(const Texture2dDesc&, const SubResourceData*) = 0;
	virtual IBuffer* CreateBuffer(const BufferDesc&, const SubResourceData&) = 0;

	virtual IRenderTargetView* CreateRenderTargetView(IResource*, const RenderTargetViewDesc&) = 0;
	virtual IDepthStencilView* CreateDepthStencilView(IResource*, const DepthStencilViewDesc&) = 0;
	virtual IShaderResourceView* CreateShaderResourceView(IResource*, const ShaderResourceViewDesc&) = 0;
	virtual IUnorderedAccessView* CreateUnorderedAccessView(IResource*, const UnorderedAccessViewDesc&) = 0;

	virtual void LoadCubeTextureResource(const char* pPath, ITexture2d** ppTexture, IShaderResourceView** ppSRV) = 0;

	virtual void SetupInputLayout(IInputLayout* pInputLayout, IBuffer* pVertexBuffer) = 0;
};

} // namespace HALgfx
} // namespace Magnet

#endif