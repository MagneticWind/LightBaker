#include "SamplerState_plat.h"
#include "ComparisonFunc_plat.h" 

namespace Magnet
{
namespace HALgfx
{
SamplerState::SamplerState(const SamplerStateDesc& descSampler) : m_desc(descSampler), m_pSamplerState(0)
{
	
}

SamplerState::~SamplerState()
{
	if (m_pSamplerState)
	{
		m_pSamplerState->Release();
		m_pSamplerState = 0;
	}
}

D3D11_TEXTURE_ADDRESS_MODE SamplerState::GetD3DAddressMode(AddressMode mode)
{
	switch(mode)
	{
	case ADDRESSMODE_WRAP:
		return D3D11_TEXTURE_ADDRESS_WRAP;
	case ADDRESSMODE_CLAMP:
		return D3D11_TEXTURE_ADDRESS_CLAMP;
	case ADDRESSMODE_BORDER:
		return D3D11_TEXTURE_ADDRESS_BORDER;
	}
}

void SamplerState::Create(ID3D11Device* pDevice)
{
	D3D11_SAMPLER_DESC desc;
	switch(m_desc.filter)
	{
	case FILTER_MIN_MAG_MIP_POINT:
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		break;
	case FILTER_MIN_MAG_MIP_LINEAR:
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		break;
	case FILTER_ANISOTROPIC:
		desc.Filter = D3D11_FILTER_ANISOTROPIC;
		break;
	}

	desc.AddressU = GetD3DAddressMode(m_desc.addressU);
	desc.AddressV = GetD3DAddressMode(m_desc.addressV);
	desc.AddressW = GetD3DAddressMode(m_desc.addressW);

	desc.MaxAnisotropy = m_desc.maxAnisotropy;
	desc.MaxLOD = m_desc.maxLod;
	desc.MinLOD = m_desc.minLod;
	desc.MipLODBias = m_desc.mipLodBias;
	desc.ComparisonFunc = GetD3DComparisonFunc(m_desc.comparisonFunc);

	pDevice->CreateSamplerState(&desc, &m_pSamplerState);
}

ID3D11SamplerState* SamplerState::GetD3DPtr()
{
	return m_pSamplerState;
}

} // namespace HALgfx
} // namespace Magnet