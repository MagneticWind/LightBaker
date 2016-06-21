#include "glew.h"		// has to be included before gl.h
#include <gl/gl.h>
#include <gl/glu.h>

#include "DeviceContext_gl.h"

#include "Buffer_gl.h"
//#include "CommandBuffer_plat.h"
//#include "ComputeShader_plat.h"
//#include "DeviceContext_plat.h"
//#include "DepthStencilState_plat.h"
//#include "DepthStencilView_plat.h"
//#include "Format_plat.h"
#include "InputLayout_gl.h"
#include "Math\Vector4f.h"
//#include "PixelShader_plat.h"
//#include "RenderTargetView_plat.h"
//#include "RasterizerState_plat.h"
//#include "SamplerState_plat.h"
//#include "ShaderResourceView_plat.h"
//#include "UnorderedAccessView_plat.h"
//#include "Texture2d_plat.h"
//#include "VertexShader_plat.h"
#include "Viewport.h"

namespace Magnet
{
	namespace HALgfx
	{
#define MAX_NUM_BUFFER 4
#define MAX_NUM_SAMPLERS 4

		//------------------------------------------------------------------
		GLDeviceContext::GLDeviceContext(HGLRC hRC) : m_hRC(hRC)
		{
			
		}

		//------------------------------------------------------------------
		GLDeviceContext::~GLDeviceContext()
		{

		}

		//------------------------------------------------------------------
		void GLDeviceContext::SetInputlayout(IInputLayout* pInputLayout)
		{
			GLInputLayout* inputlayout = static_cast<GLInputLayout*>(pInputLayout);
			const int elementCount = inputlayout->GetElementCount();

			for (int i = 0; i < elementCount; ++i)
			{
				const InputElementDesc& elementDesc = inputlayout->GetElementDesc(i);

				glVertexAttribPointer(i, elementDesc.size, GL_FLOAT, GL_FALSE, 0, 0);

			}
		}

		void GLDeviceContext::SetVertexBuffer(int iSlot, int iStride, int iOffset, IBuffer* pBuffer)
		{
			GLBuffer* pVertexBuffer = static_cast<GLBuffer*>(pBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, pVertexBuffer->GetGLHandle());
		}

		//------------------------------------------------------------------
		void GLDeviceContext::SetConstantBuffers(int iSlot, int iNumBuffers, ShaderType eType, IBuffer* pBuffer[])
		{
			

		}

		void GLDeviceContext::SetIndexBuffer(IBuffer* pBuffer, Format format, int iOffset)
		{
			GLBuffer* pIndexBuffer = static_cast<GLBuffer*>(pBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pIndexBuffer->GetGLHandle());
		}

		//------------------------------------------------------------------
		void GLDeviceContext::SetShader(ShaderType shaderType, IShader* pShader)
		{
			
		}

		//------------------------------------------------------------------
		void GLDeviceContext::DrawIndexed(unsigned int uIndexCount, unsigned int uStartIndexLocation, int iBaseVertexLocation)
		{
			glDrawArrays(m_ePrimitiveMode, uStartIndexLocation, uIndexCount);
		}

		//------------------------------------------------------------------
		void GLDeviceContext::ExuecuteCompute(unsigned int uX, unsigned uY, unsigned uZ)
		{
			
		}

		//------------------------------------------------------------------
		void GLDeviceContext::Map(IResource* pResource, unsigned int uSubResourceIndex, MapType eType, unsigned int uMapFlag, SubResourceData& data)
		{
			
		}

		//------------------------------------------------------------------
		void GLDeviceContext::Unmap(IResource* pResource, unsigned int uSubResourceIndex)
		{
			
		}

		//------------------------------------------------------------------
		void GLDeviceContext::SetRasterizerState(IRasterizerState* pState)
		{
			
		}

		//------------------------------------------------------------------
		void GLDeviceContext::SetDepthStencilState(IDepthStencilState* pState)
		{
			
		}

		//------------------------------------------------------------------
		void GLDeviceContext::SetBlendState(IBlendState* pState)
		{
		}

		//------------------------------------------------------------------
		void GLDeviceContext::SetSamplerStates(ShaderType shaderType, int iOffset, int iNumSamplers, ISamplerState* pState[])
		{
			
		}

		//------------------------------------------------------------------
		void GLDeviceContext::SetShaderResourceViews(ShaderType shaderType, int iOffset, int iNumViews, IShaderResourceView* pSRVs[])
		{
			
		}

		//------------------------------------------------------------------
		void GLDeviceContext::SetUnorderedAccessViews(ShaderType shaderType, int iOffset, int iNumViews, IUnorderedAccessView* pUAVs[])
		{
			
		}

		//------------------------------------------------------------------
		void GLDeviceContext::SetViewPort(const ViewPort& viewPort)
		{
			//glViewport(viewPort.topLeftX, viewPort.topLeftY, viewPort.width, viewPort.height);
		}

		//------------------------------------------------------------------
		void GLDeviceContext::SetRenderTargetViews(int iNumViews, IRenderTargetView* const rtvs[], IDepthStencilView* dsv)
		{
			
		}

		//------------------------------------------------------------------
		void GLDeviceContext::ClearRenderTargetView(IRenderTargetView* pRTV, const Math::Vector4f& v4Color)
		{
			glClearColor(v4Color.x, v4Color.y, v4Color.z, v4Color.w); // background color of the window
			glClear(GL_COLOR_BUFFER_BIT );			// Clear the frame buffer
		}

		//------------------------------------------------------------------
		void GLDeviceContext::ClearDepthStencilView(IDepthStencilView* pDSV, unsigned int uFlag, float fDepth, int iStencil)
		{
			glClearDepth(fDepth);
			glClear(GL_DEPTH_BUFFER_BIT);			// Clear the depth buffer
		}

		//------------------------------------------------------------------
		void GLDeviceContext::SetPrimitiveTopology(PrimitiveTopology eType)
		{
			switch (eType)
			{
			case PRIMITIVE_TOPOLOGY_TRIANGLELIST:
				m_ePrimitiveMode = GL_TRIANGLES;
				break;
			case PRIMITIVE_TOPOLOGY_LINELIST:
				m_ePrimitiveMode = GL_LINES;
				break;
			case PRIMITIVE_TOPOLOGY_POINTLIST:
				m_ePrimitiveMode = GL_POINTS;
				break;
			case PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
				m_ePrimitiveMode = GL_TRIANGLE_STRIP;
			default:
				break;
			}
		}

		void GLDeviceContext::BeginEvent(const char* debugInfo)
		{

		}

		void GLDeviceContext::EndEvent()
		{

		}

		ICommandBuffer* GLDeviceContext::FinishCommandBuffer()
		{
			return 0;
		}

		void GLDeviceContext::ExecuteCommandBuffer(ICommandBuffer* pCommandBuffer, bool bRestoreContextState)
		{
			
		}

	} // namespace HALgfx
} // namespace Magnet