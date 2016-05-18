#ifndef IDEVICE_CONTEXT_H
#define IDEVICE_CONTEXT_H

#include "ShaderType.h"
#include "IResource.h"
#include "Format.h"

namespace Magnet
{

namespace Math
{
class Vector4f;
}

namespace HALgfx
{

class IResource;
struct SubResourceData;
class IInputLayout;
class IBuffer;
class ICommandBuffer;
class IRasterizerState;
class IDepthStencilState;
class IBlendState;
class IRenderTargetView;
class IDepthStencilView;
class IShader;
class ISamplerState;
class IShaderResourceView;
class IUnorderedAccessView;
class IRasterzerState;

struct ViewPort;

enum ClearFlag
{
	CLEAR_DEPTH = 1u,
	CLEAR_STENCIL = (1u << 1)
};

enum PrimitiveTopology
{
	PRIMITIVE_TOPOLOGY_UNDEFINED = 0,
	PRIMITIVE_TOPOLOGY_POINTLIST,
	PRIMITIVE_TOPOLOGY_LINELIST, 
	PRIMITIVE_TOPOLOGY_LINESTRIP,
	PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
};

class IDeviceContext
{
public:
	virtual ~IDeviceContext(){}
	//virtual void CopyResource(IResource* pDest, IResource* pSource) = 0;
	//virtual void CopySubResource(IResource* pDest, int iDestIndex, IResource* pSource, int iSourceIndex) = 0;
	//virtual void UpdateSubResource(IResource* pDest, const SubResourceData&, int iDestIndex) = 0;

	virtual void DrawIndexed(unsigned int uIndexCount, unsigned int uStartIndexLocation, int iBaseVertexLocation) = 0;

	virtual void ExuecuteCompute(unsigned int uX, unsigned int uY, unsigned int uZ) = 0;

	virtual void SetInputlayout(IInputLayout* pInputLayout) = 0;
	virtual void SetVertexBuffer(int iSlot, int iStride, int iOffset, IBuffer* pBuffer) = 0;
	virtual void SetConstantBuffers(int iSlot, int iNumBuffers, ShaderType eType,IBuffer* pBuffer[]) = 0;
	virtual void SetIndexBuffer(IBuffer* pBuffer, Format format, int iOffset) = 0;

	virtual void SetRasterizerState(IRasterizerState* pState) = 0;
	virtual void SetDepthStencilState(IDepthStencilState* pState) = 0;
	virtual void SetBlendState(IBlendState* pState) = 0;
	virtual void SetSamplerStates(ShaderType shaderType, int iOffset, int iNumSamplers, ISamplerState* pState[]) = 0;

	virtual void SetRenderTargetViews(int iNumViews, IRenderTargetView* const rtvs[], IDepthStencilView* dsv) = 0;

	virtual void SetShader(ShaderType shaderType, IShader* pShader) = 0;
	virtual void SetShaderResourceViews(ShaderType shaderType, int iOffset, int iNumViews, IShaderResourceView* pSRVs[]) = 0;
	virtual void SetUnorderedAccessViews(ShaderType shaderType, int iOffset, int iNumViews, IUnorderedAccessView* pUAVs[]) = 0;
	virtual void SetViewPort(const ViewPort& viewPort) = 0;

	virtual void SetPrimitiveTopology(PrimitiveTopology eType) = 0;

	virtual void ClearRenderTargetView(IRenderTargetView* pRTV, const Math::Vector4f& v4Color) = 0;
	virtual void ClearDepthStencilView(IDepthStencilView* pDSV, unsigned int uFlag, float fDepth, int iStencil) = 0;

	virtual void Map(IResource* pResource, unsigned int uSubResourceIndex, MapType eType, unsigned int uMapFlag, SubResourceData& data) = 0;
	virtual void Unmap(IResource* pResource, unsigned int uSubResourceIndex) = 0;

	virtual void BeginEvent(const char* debugInfo) = 0;
	virtual void EndEvent() = 0;

	virtual ICommandBuffer* FinishCommandBuffer() = 0;
	virtual void ExecuteCommandBuffer(ICommandBuffer* pCommandBuffer, bool bRestoreContextState) = 0;

};

} // namespace HALgfx
} // namespace Magnet
#endif