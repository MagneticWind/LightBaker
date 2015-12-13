#include <assert.h>
#include <stdio.h>

#include "DepthStencilView_plat.h"
#include "Texture2d_plat.h"
#include "Format_plat.h"

namespace Magnet
{
namespace HALgfx
{

//------------------------------------------------------------------
DepthStencilView::DepthStencilView()
{
	m_pDSV = 0;
}

//------------------------------------------------------------------
DepthStencilView::~DepthStencilView()
{
	if (m_pDSV)
	{
		m_pDSV->Release();
		m_pDSV = 0;
	}
}

//------------------------------------------------------------------
void DepthStencilView::Create(const DepthStencilViewDesc& desc, IResource* pResource, ID3D11Device* pDevice)
{
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = GetD3DFormat(desc.format);
	descDSV.Flags = desc.flags;

	ID3D11Resource* pD3DResource = 0;
	switch(desc.viewDimension)
	{
	case DSV_DIMENSION_TEXTURE2D:
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = desc.texture2d.mipSlice;
		Texture2d* pTexture2d = static_cast<Texture2d*>(pResource);
		pD3DResource = pTexture2d->GetD3DPtr();
		break;
	};

	HRESULT hr = pDevice->CreateDepthStencilView( pD3DResource, &descDSV, &m_pDSV );
	if (FAILED(hr))
	{
		printf("Failed To Create depth stencil view.");
		assert(0);
	}
	
}

//------------------------------------------------------------------
ID3D11DepthStencilView* DepthStencilView::GetD3DPtr()
{
	return m_pDSV;
}

} // namespace HALgfx
} // namespace Magnet