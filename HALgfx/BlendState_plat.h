#ifndef BLEND_STATE_H
#define BLEND_STATE_H

#include "IBlendState.h"
#include <d3d11.h>

namespace Magnet
{
namespace HALgfx
{

class BlendState : public IBlendState
{
public:
	BlendState(const BlendDesc& blendDesc);
	void Create(ID3D11Device* pDevice);
	~BlendState();

private:
	D3D11_BLEND_OP GetD3DBlendOp(BlendOp blendOp);
	D3D11_BLEND GetD3DBlend(Blend blend);

private:
	BlendDesc m_desc;
	ID3D11BlendState* m_pBlendState;
};

} // namespace HALgfx
} // namespace Magnet

#endif