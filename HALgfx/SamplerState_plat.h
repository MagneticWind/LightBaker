#ifndef SAMPLER_STATE_H
#define SAMPLER_STATE_H

#include <d3d11.h>
#include "ISamplerState.h"

namespace Magnet
{
namespace HALgfx
{
class SamplerState : public ISamplerState
{
public:
	SamplerState(const SamplerStateDesc& descSampler);
	~SamplerState();
	void Create(ID3D11Device* pDevice);
	ID3D11SamplerState* GetD3DPtr();

private:
	D3D11_TEXTURE_ADDRESS_MODE GetD3DAddressMode(AddressMode mode);

private:
	SamplerStateDesc m_desc;
	ID3D11SamplerState* m_pSamplerState;
};
} // namespace HALgfx
} // namespace Magnet

#endif