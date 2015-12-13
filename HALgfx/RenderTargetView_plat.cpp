#include <assert.h>
#include <stdio.h>

#include "RenderTargetView_plat.h"
#include "Texture2d_plat.h"
#include "Format_plat.h"

namespace Magnet
{
namespace HALgfx
{
//------------------------------------------------------------------
RenderTargetView::RenderTargetView() : m_pRTV(0)
{
}

//------------------------------------------------------------------
RenderTargetView::~RenderTargetView()
{
	if (m_pRTV)
	{
		m_pRTV->Release();
		m_pRTV = 0;
	}
}

//------------------------------------------------------------------
void RenderTargetView::Create(const RenderTargetViewDesc& desc, IResource* pResource, ID3D11Device* pd3dDevice)
{
	D3D11_RENDER_TARGET_VIEW_DESC d3dDesc;
	d3dDesc.Format = GetD3DFormat(desc.format);
	ID3D11Resource* pD3DResource = 0;
	switch(desc.viewDimension)
	{
	case RTV_DIMENSION_TEXTURE2D:
		d3dDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		d3dDesc.Texture2D.MipSlice = 0;
		Texture2d* pTexture2d = static_cast<Texture2d*>(pResource);
		pD3DResource = pTexture2d->GetD3DPtr();
		break;
	}
	pd3dDevice->CreateRenderTargetView(pD3DResource, &d3dDesc, &m_pRTV);
}

//------------------------------------------------------------------
void RenderTargetView::Create(IDXGISwapChain* swapChain, ID3D11Device* pd3dDevice)
{
	m_pSwapChain = swapChain;
	ID3D11Texture2D* pTexture2d;
	HRESULT hr = swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )& pTexture2d );
	if (FAILED(hr))
	{
		printf("Failed to get the back buffer.");
		assert(0);
	}

	hr = pd3dDevice->CreateRenderTargetView(pTexture2d, 0, &m_pRTV);
	if (FAILED(hr))
	{
		printf("Failed to create the render target view.");
		assert(0);
	}
}

//------------------------------------------------------------------
ID3D11RenderTargetView* RenderTargetView::GetD3DPtr()
{
	return m_pRTV;
}

} // namespace HALgfx
} // namespace Magnet