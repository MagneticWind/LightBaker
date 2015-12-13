#ifndef DEVICE_CONTEXT_h
#define DEVICE_CONTEXT_H

#include "IDeviceContext.h"

namespace Magnet
{
namespace HALgfx
{

class DeviceContext
{
public:

	virtual void CopyResource(IResource* pDest, IResource* pSource) = 0;
	virtual void CopySubResource(IResource* pDest, int iDestIndex, IResource* pSource, int iSourceIndex) = 0;
	virtual void UpdateSubResource(IResource* pDest, const SubResourceData&, int iDestIndex) = 0;

	virtual void Draw(const DrawIndexedParams&, bool flushScope) = 0;

	virtual void ExuecuteCompute(unsigned int uX, unsigned int uY, unsigned int uZ) = 0;

	virtual void SetInputlayout(IInputLayout* pInputLayout) = 0;
	virtual void SetVertexBuffer(int iSlot, int iStride, int iOffset, IBuffer* pBuffer) = 0;
	virtual void SetConstantBuffer(int iSlot, int iStride, int iOffset, IBuffer* pBuffer) = 0;
	virtual void SetIndexBuffer(IBuffer* pBuffer) = 0;

	virtual void SetRasterizationState(IRasterizerState* pState) = 0;
	virtual void SetDepthStencilState(IDepthStencilState* pState) = 0;
	virtual void SetBlendState(IBlendState* pState) = 0;

	virtual void SetOutputViews(int iNumViews, IRenderTargetView* const rtvs[], IDepthStencilView* dsv) = 0;
	virtual void SetRenderTargetView(int iIndex, IRenderTargetView* pView) = 0;
	virtual void SetDepthStencilView(IDepthStencilView* pView) = 0;

	virtual void SetShader(ShaderType shaderType, IShader* pShader) = 0;
	virtual void SetSampler(ShaderType shaderType, ISamplerState* pState) = 0;
	virtual void SetTexture(ShaderType shaderType, ITexture* pTexture) = 0;

	virtual bool Map(IResource* pResource, const SubResourceIndex&, unsigned int mapFlags, SubResourceData&) = 0;



};

} // namespace HALgfx
} // namespace Magnet

#endif