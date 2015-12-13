#ifndef RASTERIZER_STATE_H
#define RASTERIZER_STATE_H

#include "IRasterizerState.h"
#include <d3d11.h>

namespace Magnet
{
namespace HALgfx
{
class RasterizerState : public IRasterizerState
{
public:
	RasterizerState();
	~RasterizerState();

	void Create(const RasterizerDesc& descRS, ID3D11Device* pDevice);

	ID3D11RasterizerState* GetD3DPtr();

private:
	ID3D11RasterizerState* m_pRasterizerState;
};
} // namespace HALgfx
} // namespace Magnet

#endif