#include <d3d9.h>

#include "Buffer_plat.h"
#include "DeviceContext_plat.h"
#include "DepthStencilState_plat.h"
#include "DepthStencilView_plat.h"
#include "Format_plat.h"
#include "InputLayout_plat.h"
#include "Math\Vector4f.h"
#include "PixelShader_plat.h"
#include "RenderTargetView_plat.h"
#include "RasterizerState_plat.h"
#include "SamplerState_plat.h"
#include "ShaderResourceView_plat.h"
#include "Texture2d_plat.h"
#include "VertexShader_plat.h"
#include "Viewport.h"

namespace Magnet
{
namespace HALgfx
{
#define MAX_NUM_BUFFER 4
#define MAX_NUM_SAMPLERS 4

//------------------------------------------------------------------
DeviceContext::DeviceContext(ID3D11DeviceContext* pContext)
{
	m_pDeviceContext = pContext;
}

//------------------------------------------------------------------
DeviceContext::~DeviceContext()
{
	if (m_pDeviceContext)
	{
		m_pDeviceContext->Release();
		m_pDeviceContext = 0;
	}
}

//------------------------------------------------------------------
void DeviceContext::SetInputlayout(IInputLayout* pInputLayout)
{
	InputLayout* pInputLayoutPlat = static_cast<InputLayout*>(pInputLayout);
	m_pDeviceContext->IASetInputLayout(pInputLayoutPlat->GetD3DPtr());
}

void DeviceContext::SetVertexBuffer(int iSlot, int iStride, int iOffset, IBuffer* pBuffer)
{
	if (pBuffer == 0)
	{
		m_pDeviceContext->IASetVertexBuffers(0, 0, 0, 0, 0);
		return;
	}

	unsigned int uStrides[1];
	unsigned int uOffsets[1];
	uStrides[0] = iStride;
	uOffsets[0] = iOffset;
	Buffer* pBufferPlat = static_cast<Buffer*>(pBuffer);
	ID3D11Buffer* pD3DBuffer = pBufferPlat->GetD3DPtr();
	m_pDeviceContext->IASetVertexBuffers(iSlot, 1, &pD3DBuffer, uStrides, uOffsets);
}

//------------------------------------------------------------------
void DeviceContext::SetConstantBuffers(int iSlot, int iNumBuffers, ShaderType eType,IBuffer* pBuffer[])
{
	ID3D11Buffer* pD3DBuffer[MAX_NUM_BUFFER];
	for(int i = 0; i < iNumBuffers; ++i)
	{
		Buffer* pBufferPlat = static_cast<Buffer*>(pBuffer[i]);

		if (pBufferPlat == 0)
			pD3DBuffer[i] = 0;
		else
			pD3DBuffer[i] = pBufferPlat->GetD3DPtr();
	}

	switch(eType)
	{
	case VERTEX_SHADER:
		m_pDeviceContext->VSSetConstantBuffers(iSlot, iNumBuffers, pD3DBuffer);
		return;
	case PIXEL_SHADER:
		m_pDeviceContext->PSSetConstantBuffers(iSlot, iNumBuffers, pD3DBuffer);
		return;
	}

}

void DeviceContext::SetIndexBuffer(IBuffer* pBuffer, Format format, int iOffset)
{
	if (pBuffer == 0)
	{
		m_pDeviceContext->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
		return;
	}

	Buffer* pBufferPlat = static_cast<Buffer*>(pBuffer);
	ID3D11Buffer* pD3DBuffer = pBufferPlat->GetD3DPtr();
	DXGI_FORMAT d3dFormat = GetD3DFormat(format);
	m_pDeviceContext->IASetIndexBuffer(pD3DBuffer, d3dFormat, iOffset);
}

//------------------------------------------------------------------
void DeviceContext::SetShader(ShaderType shaderType, IShader* pShader)
{
	if(shaderType == VERTEX_SHADER)
	{
		if(pShader)
		{
			VertexShader* pVertexShader = static_cast<VertexShader*>(pShader);
			ID3D11VertexShader* pD3DShader = pVertexShader->GetD3DPtr();
			m_pDeviceContext->VSSetShader(pD3DShader, 0, 0);
		}
		else
		{
			m_pDeviceContext->VSSetShader(0, 0, 0);
		}
	}
	else if (shaderType == PIXEL_SHADER)
	{
		if(pShader)
		{
			PixelShader* pPixelShader = static_cast<PixelShader*>(pShader);
			ID3D11PixelShader* pD3DShader = pPixelShader->GetD3DPtr();
			m_pDeviceContext->PSSetShader(pD3DShader, 0, 0);
		}
		else
		{
			m_pDeviceContext->PSSetShader(0, 0, 0);
		}
	}
}

//------------------------------------------------------------------
void DeviceContext::DrawIndexed(unsigned int uIndexCount, unsigned int uStartIndexLocation, int iBaseVertexLocation)
{
	m_pDeviceContext->DrawIndexed(uIndexCount, uStartIndexLocation, iBaseVertexLocation);
}

//------------------------------------------------------------------
void DeviceContext::Map(IResource* pResource, unsigned int uSubResourceIndex, MapType eType, unsigned int uMapFlag, SubResourceData& data)
{
	ResourceType resourceType = pResource->GetType();
	ID3D11Resource* pD3DResource;
	switch(resourceType)
	{
	case RESOURCE_BUFFER:
		{
			Buffer* pBuffer = static_cast<Buffer*>(pResource);
			pD3DResource = pBuffer->GetD3DPtr();
			break;
		}
	case RESOURCE_TEXTURE2D:
		{
			Texture2d* pTexture2d = static_cast<Texture2d*>(pResource);
			pD3DResource = pTexture2d->GetD3DPtr();
			break;
		}
	}

	D3D11_MAP map;
	switch(eType)
	{
	case MAP_READ:
		map = D3D11_MAP_READ;
		break;
	case MAP_WRITE:
		map = D3D11_MAP_WRITE;
		break;
	case MAP_READ_WRITE:
		map = D3D11_MAP_READ_WRITE;
		break;
	case MAP_WRITE_DISCARD:
		map = D3D11_MAP_WRITE_DISCARD;
		break;
	case MAP_WRITE_NO_OVERWRITE:
		map = D3D11_MAP_WRITE_NO_OVERWRITE;
		break;
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	m_pDeviceContext->Map(pD3DResource, uSubResourceIndex, map, uMapFlag, &mappedResource);

	data.pMem = mappedResource.pData;
	data.uMemPitch = mappedResource.RowPitch;
	data.uMemSlicePitch = mappedResource.DepthPitch;
}

//------------------------------------------------------------------
void DeviceContext::Unmap(IResource* pResource, unsigned int uSubResourceIndex)
{
	ResourceType resourceType = pResource->GetType();
	ID3D11Resource* pD3DResource;

	switch(resourceType)
	{
	case RESOURCE_BUFFER:
		{
			Buffer* pBuffer = static_cast<Buffer*>(pResource);
			pD3DResource = pBuffer->GetD3DPtr();
			break;
		}
	case RESOURCE_TEXTURE2D:
		{
			Texture2d* pTexture2d = static_cast<Texture2d*>(pResource);
			pD3DResource = pTexture2d->GetD3DPtr();
			break;
		}
	}
	m_pDeviceContext->Unmap(pD3DResource, uSubResourceIndex);
}

//------------------------------------------------------------------
void DeviceContext::SetRasterizerState(IRasterizerState* pState)
{
	RasterizerState* pRasterizerState = static_cast<RasterizerState*>(pState);
	ID3D11RasterizerState * pD3DRasterizerState = pRasterizerState->GetD3DPtr();

	m_pDeviceContext->RSSetState(pD3DRasterizerState);
}

//------------------------------------------------------------------
void DeviceContext::SetDepthStencilState(IDepthStencilState* pState)
{
	DepthStencilState* pDepthStencilState = static_cast<DepthStencilState*>(pState);
	m_pDeviceContext->OMSetDepthStencilState(pDepthStencilState->GetD3DPtr(), 0);
}

//------------------------------------------------------------------
void DeviceContext::SetBlendState(IBlendState* pState)
{
}

//------------------------------------------------------------------
void DeviceContext::SetSamplerStates(ShaderType shaderType, int iOffset, int iNumSamplers, ISamplerState* pState[])
{
	ID3D11SamplerState* pD3DSamplerStates[MAX_NUM_SAMPLERS];
	for (int i = 0; i < iNumSamplers; ++i)
	{
		pD3DSamplerStates[i] = static_cast<SamplerState*>(pState[i])->GetD3DPtr();
	}
	switch(shaderType)
	{
	case PIXEL_SHADER:
		m_pDeviceContext->PSSetSamplers(iOffset, iNumSamplers, pD3DSamplerStates);
		break;
	}
}

//------------------------------------------------------------------
void DeviceContext::SetShaderResourceViews(ShaderType shaderType, int iOffset, int iNumViews, IShaderResourceView* pSRVs[])
{
	ID3D11ShaderResourceView* pD3Dsrvs[MAX_NUM_SRVS];

	for (int i = 0; i < iNumViews; ++i)
	{
		if (pSRVs[i] != 0)
		{
			ShaderResourceView* pSRV = static_cast<ShaderResourceView*>(pSRVs[i]);
			pD3Dsrvs[i] = pSRV->GetD3DPtr();
		}
		else
		{
			pD3Dsrvs[i] = 0;
		}
	}
	
	switch(shaderType)
	{
	case PIXEL_SHADER:
		m_pDeviceContext->PSSetShaderResources(iOffset, iNumViews, pD3Dsrvs);
	}
}

//------------------------------------------------------------------
void DeviceContext::SetViewPort(const ViewPort& viewPort)
{
	D3D11_VIEWPORT d3dViewPort;
	d3dViewPort.Width = viewPort.width;
	d3dViewPort.Height = viewPort.height;
	d3dViewPort.MinDepth = viewPort.minDepth;
	d3dViewPort.MaxDepth = viewPort.maxDepth;
	d3dViewPort.TopLeftX = viewPort.topLeftX;
	d3dViewPort.TopLeftY = viewPort.topLeftY;
	
	m_pDeviceContext->RSSetViewports(1, &d3dViewPort);
}

//------------------------------------------------------------------
void DeviceContext::SetRenderTargetViews(int iNumViews, IRenderTargetView* const rtvs[], IDepthStencilView* dsv)
{
	DepthStencilView* pDSV = static_cast<DepthStencilView*>(dsv);

	ID3D11RenderTargetView* pRtvs[MAX_NUM_RTVS];
	for(int i = 0; i < iNumViews; ++i)
	{
		RenderTargetView* pRTV = static_cast<RenderTargetView*>(rtvs[i]);
		pRtvs[i] = pRTV->GetD3DPtr();
	}

	if (iNumViews == 0)
	{
		m_pDeviceContext->OMSetRenderTargets(0, 0, pDSV->GetD3DPtr());
	}
	else
	{
		m_pDeviceContext->OMSetRenderTargets(iNumViews, pRtvs, pDSV->GetD3DPtr());
	}
}

//------------------------------------------------------------------
void DeviceContext::ClearRenderTargetView(IRenderTargetView* pRTV, const Math::Vector4f& v4Color)
{
	RenderTargetView* pD3DRTV = static_cast<RenderTargetView*>(pRTV);
	float faColor[4] = {v4Color.x, v4Color.y, v4Color.z, v4Color.w};
	m_pDeviceContext->ClearRenderTargetView(pD3DRTV->GetD3DPtr(), faColor);
}

//------------------------------------------------------------------
void DeviceContext::ClearDepthStencilView(IDepthStencilView* pDSV, unsigned int uFlag, float fDepth, int iStencil)
{
	DepthStencilView* pD3DDSV = static_cast<DepthStencilView*>(pDSV);
	m_pDeviceContext->ClearDepthStencilView(pD3DDSV->GetD3DPtr(), uFlag, fDepth, iStencil);
}

//------------------------------------------------------------------
ID3D11DeviceContext* DeviceContext::GetD3DPtr()
{
	return m_pDeviceContext;
}

//------------------------------------------------------------------
void DeviceContext::SetPrimitiveTopology(PrimitiveTopology eType)
{
	D3D11_PRIMITIVE_TOPOLOGY ePrimitive;
	switch(eType)
	{
	case PRIMITIVE_TOPOLOGY_UNDEFINED:
		ePrimitive = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
		break;
	case PRIMITIVE_TOPOLOGY_POINTLIST:
		ePrimitive = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		break;
	case PRIMITIVE_TOPOLOGY_LINELIST:
		ePrimitive = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		break;
	case PRIMITIVE_TOPOLOGY_LINESTRIP:
		ePrimitive = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
		break;
	case PRIMITIVE_TOPOLOGY_TRIANGLELIST:
		ePrimitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		break;
	case PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		ePrimitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		break;
	}
	m_pDeviceContext->IASetPrimitiveTopology(ePrimitive);
}

void DeviceContext::BeginEvent(const char* debugInfo)
{
	size_t newsize = strlen(debugInfo) + 1;
	wchar_t * wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, debugInfo, _TRUNCATE);

	D3DPERF_BeginEvent(D3DCOLOR_XRGB(128, 128, 128), wcstring);
}

void DeviceContext::EndEvent()
{
	D3DPERF_EndEvent();
}

} // namespace HALgfx
} // namespace Magnet