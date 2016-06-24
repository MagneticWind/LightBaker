#ifndef GL_DEVICE_CONTEXT_H
#define GL_DEVICE_CONTEXT_H

#include <Windows.h>
#include "IDeviceContext.h"

namespace Magnet
{
namespace HALgfx
{
struct ViewPort;

class GLDeviceContext : public IDeviceContext
{
public:

	GLDeviceContext(HGLRC hRC);
	~GLDeviceContext();

	//	virtual void CopyResource(IResource* pDest, IResource* pSource);
	//	virtual void CopySubResource(IResource* pDest, int iDestIndex, IResource* pSource, int iSourceIndex);
	//	virtual void UpdateSubResource(IResource* pDest, const SubResourceData&, int iDestIndex);

	virtual void DrawIndexed(unsigned int uIndexCount, unsigned int uStartIndexLocation, int iBaseVertexLocation);

	virtual void ExuecuteCompute(unsigned int uX, unsigned int uY, unsigned int uZ);

	virtual void SetInputlayout(IInputLayout* pInputLayout);
	virtual void SetVertexBuffer(int iSlot, int iStride, int iOffset, IBuffer* pBuffer);
	virtual void SetConstantBuffers(int iSlot, int iNumBuffers, ShaderType eType, IBuffer* pBuffer[]);
	virtual void SetIndexBuffer(IBuffer* pBuffer, Format format, int iOffset);

	virtual void SetRasterizerState(IRasterizerState* pState);
	virtual void SetDepthStencilState(IDepthStencilState* pState);
	virtual void SetBlendState(IBlendState* pState);

	virtual void SetRenderTargetViews(int iNumViews, IRenderTargetView* const rtvs[], IDepthStencilView* dsv);

	virtual void SetShader(ShaderType shaderType, IShader* pShader);
	virtual void SetSamplerStates(ShaderType shaderType, int iOffset, int iNumSamplers, ISamplerState* pState[]);
	virtual void SetShaderResourceViews(ShaderType shaderType, int iOffset, int iNumViews, IShaderResourceView* pSRVs[]);
	virtual void SetUnorderedAccessViews(ShaderType shaderType, int iOffset, int iNumViews, IUnorderedAccessView* pUAVs[]);
	virtual void SetViewPort(const ViewPort& viewPort);

	virtual void SetPrimitiveTopology(PrimitiveTopology eType);

	virtual void Map(IResource* pResource, unsigned int uSubResourceIndex, MapType eType, unsigned int uMapFlag, SubResourceData& data);
	virtual void Unmap(IResource* pResource, unsigned int uSubResourceIndex);

	void ClearRenderTargetView(IRenderTargetView* pRTV, const Math::Vector4f& v4Color);
	void ClearDepthStencilView(IDepthStencilView* pDSV, unsigned int uFlag, float fDepth, int iStencil);

	virtual void BeginEvent(const char* debugInfo);
	virtual void EndEvent();

	virtual ICommandBuffer* FinishCommandBuffer();
	virtual void ExecuteCommandBuffer(ICommandBuffer* pCommandBuffer, bool bRestoreContextState);

	void SetCurrentProgram(GLuint program);

private:
	HGLRC m_hRC;
	GLenum m_ePrimitiveMode;

	GLuint m_currentProgram;

};

} // namespace HALgfx
} // namespace Magnet
#endif