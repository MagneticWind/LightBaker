#include "Texture2d_plat.h"
#include "Format_plat.h"
#include "IDevice.h"

namespace Magnet
{
namespace HALgfx
{
Texture2d::Texture2d() : m_pTexture2D(0)
{

}

Texture2d::Texture2d(ID3D11Texture2D* pTexture2D) : m_pTexture2D(pTexture2D)
{

}

Texture2d::~Texture2d()
{
	if (m_pTexture2D)
	{
		m_pTexture2D->Release();
		m_pTexture2D = 0;
	}
}

void Texture2d::Create(const Texture2dDesc& texture2dDesc, const SubResourceData* subResourceData, ID3D11Device* pDevice)
{

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = texture2dDesc.width;
	desc.Height = texture2dDesc.height;
	desc.MipLevels = texture2dDesc.mipLevels;
	desc.ArraySize = texture2dDesc.arraySize;
	desc.CPUAccessFlags = texture2dDesc.cpuAccessFlags;
	desc.BindFlags = texture2dDesc.bindFlags;
	desc.MiscFlags = texture2dDesc.miscFlags;
	desc.Format = GetD3DFormat(texture2dDesc.format);
	desc.SampleDesc.Count = texture2dDesc.sampleDesc.uCount;
	desc.SampleDesc.Quality = texture2dDesc.sampleDesc.uQuality;

	switch(texture2dDesc.usage)
	{
	case USAGE_DEFAULT:
		desc.Usage = D3D11_USAGE_DEFAULT;
		break;
	case USAGE_DYNAMIC:
		desc.Usage = D3D11_USAGE_DYNAMIC;
		break;
	};

	//cubemap
	if (desc.ArraySize == 6)
	{
		D3D11_SUBRESOURCE_DATA initData[6];
		for (int i = 0; i < 6; ++i)
		{
			initData[i].pSysMem = subResourceData[i].pMem;
			initData[i].SysMemPitch = subResourceData[i].uMemPitch;
			initData[i].SysMemSlicePitch = subResourceData[i].uMemSlicePitch;
		}

		pDevice->CreateTexture2D(&desc, initData, &m_pTexture2D);
	}
	else
	{
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = subResourceData->pMem;
		initData.SysMemPitch = subResourceData->uMemPitch;
		initData.SysMemSlicePitch = subResourceData->uMemSlicePitch;

		if (initData.pSysMem)
			pDevice->CreateTexture2D(&desc, &initData, &m_pTexture2D);
		else
			pDevice->CreateTexture2D(&desc, 0, &m_pTexture2D);
	}
}

} // namespace HALgfx
} // namespace Magnet