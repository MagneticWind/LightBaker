#include "ShaderResourceView_plat.h"
#include "Texture2d_plat.h"
#include "Format_plat.h"

namespace Magnet
{
namespace HALgfx
{
//------------------------------------------------------------------
ShaderResourceView::ShaderResourceView() : m_pSRV(0)
{

}

//------------------------------------------------------------------
ShaderResourceView::ShaderResourceView(ID3D11ShaderResourceView* pSRV) : m_pSRV(pSRV)
{

}

//------------------------------------------------------------------
ShaderResourceView::~ShaderResourceView()
{
	if (m_pSRV)
	{
		m_pSRV->Release();
		m_pSRV = 0;
	}
}

//------------------------------------------------------------------
void ShaderResourceView::Create(const ShaderResourceViewDesc& srvDesc, IResource* pResource, ID3D11Device* pDevice)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	desc.Format = GetD3DFormat(srvDesc.format);
	ID3D11Resource* pD3DResource;
	switch(srvDesc.viewDimension)
	{
	case SRV_DIMENSION_TEXTURE2D:
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = srvDesc.texSRV.mipLevels;
		desc.Texture2D.MostDetailedMip = srvDesc.texSRV.mostDetailedMip;
		Texture2d* pTexture2d = static_cast<Texture2d*>(pResource);
		pD3DResource = pTexture2d->GetD3DPtr();
		break;
	};

	pDevice->CreateShaderResourceView(pD3DResource , &desc, &m_pSRV);

}

//------------------------------------------------------------------
ID3D11ShaderResourceView* ShaderResourceView::GetD3DPtr()
{
	return m_pSRV;
}

} // namespace HALgfx
} // namespace Magnet