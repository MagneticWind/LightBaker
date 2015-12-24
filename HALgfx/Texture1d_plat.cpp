#include "Texture1d_plat.h"
#include "Format_plat.h"
#include "IDevice.h"

namespace Magnet
{
namespace HALgfx
{
Texture1d::Texture1d() : m_pTexture1D(0)
{

}

Texture1d::Texture1d(ID3D11Texture1D* pTexture1D) : m_pTexture1D(pTexture1D)
{

}

Texture1d::~Texture1d()
{
	if (m_pTexture1D)
	{
		m_pTexture1D->Release();
		m_pTexture1D = 0;
	}
}

void Texture1d::Create(const Texture1dDesc& texture1dDesc, const SubResourceData& subResourceData, ID3D11Device* pDevice)
{
	D3D11_TEXTURE1D_DESC desc;
	desc.Width = texture1dDesc.width;
	desc.MipLevels = texture1dDesc.mipLevels;
	desc.ArraySize = texture1dDesc.arraySize;
	desc.CPUAccessFlags = texture1dDesc.cpuAccessFlags;
	desc.BindFlags = texture1dDesc.bindFlags;
	desc.MiscFlags = texture1dDesc.miscFlags;
	desc.Format = GetD3DFormat(texture1dDesc.format);

	switch (texture1dDesc.usage)
	{
	case USAGE_DEFAULT:
		desc.Usage = D3D11_USAGE_DEFAULT;
		break;
	case USAGE_DYNAMIC:
		desc.Usage = D3D11_USAGE_DYNAMIC;
		break;
	};

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = subResourceData.pMem;
	initData.SysMemPitch = subResourceData.uMemPitch;
	initData.SysMemSlicePitch = subResourceData.uMemSlicePitch;

	if (initData.pSysMem)
		pDevice->CreateTexture1D(&desc, &initData, &m_pTexture1D);
	else
		pDevice->CreateTexture1D(&desc, 0, &m_pTexture1D);

}

} // namespace HALgfx
} // namespace Magnet