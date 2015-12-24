#include "UnorderedAccessView_plat.h"
#include "Texture2d_plat.h"
#include "Format_plat.h"
#include "Buffer_plat.h"

namespace Magnet
{
namespace HALgfx
{
//------------------------------------------------------------------
UnorderedAccessView::UnorderedAccessView() : m_pUAV(0)
{

}

//------------------------------------------------------------------
UnorderedAccessView::UnorderedAccessView(ID3D11UnorderedAccessView* pUAV) : m_pUAV(pUAV)
{

}

//------------------------------------------------------------------
UnorderedAccessView::~UnorderedAccessView()
{
	if (m_pUAV)
	{
		m_pUAV->Release();
		m_pUAV = 0;
	}
}

//------------------------------------------------------------------
void UnorderedAccessView::Create(const UnorderedAccessViewDesc& uavDesc, IResource* pResource, ID3D11Device* pDevice)
{
	D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
	desc.Format = GetD3DFormat(uavDesc.format);
	ID3D11Resource* pD3DResource;
	switch (uavDesc.viewDimension)
	{
	case UAV_DIMENSION_TEXTURE2D:
		{
			desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = uavDesc.tex2DUAV.mipSlice;
			Texture2d* pTexture2d = static_cast<Texture2d*>(pResource);
			pD3DResource = pTexture2d->GetD3DPtr();
			break;
		}
	case D3D11_UAV_DIMENSION_BUFFER:
		{
			desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			desc.Buffer.FirstElement = uavDesc.bufferUAV.firstElement;
			desc.Buffer.NumElements = uavDesc.bufferUAV.numElements;
			desc.Buffer.Flags = uavDesc.bufferUAV.flags;
			Buffer* pBuffer = static_cast<Buffer*>(pResource);
			pD3DResource = pBuffer->GetD3DPtr();
			break;
		}
	};

	pDevice->CreateUnorderedAccessView(pD3DResource, &desc, &m_pUAV);

}

//------------------------------------------------------------------
ID3D11UnorderedAccessView* UnorderedAccessView::GetD3DPtr()
{
	return m_pUAV;
}

} // namespace HALgfx
} // namespace Magnet