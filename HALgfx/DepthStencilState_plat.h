#ifndef DEPTH_STENCIL_STATE_H
#define DEPTH_STENCIL_STATE_H

#include "IDepthStencilState.h"
#include <d3d11.h>

namespace Magnet
{
namespace HALgfx
{
class DepthStencilState : public IDepthStencilState
{
public:
	DepthStencilState(const DepthStencilDesc& descDS);
	~DepthStencilState();
	void Create(ID3D11Device* pDevice);
	ID3D11DepthStencilState* GetD3DPtr();

private:
	D3D11_STENCIL_OP GetD3DStencilOp(StencilOP op);

private:
	DepthStencilDesc m_desc;
	ID3D11DepthStencilState* m_pDepthStencilState;
};

inline ID3D11DepthStencilState* DepthStencilState::GetD3DPtr()
{
	return m_pDepthStencilState;
}
} // namespace HALgfx
} // namespace Magnet

#endif