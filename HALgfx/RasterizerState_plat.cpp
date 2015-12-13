#include "RasterizerState_plat.h"

namespace Magnet
{
namespace HALgfx
{
RasterizerState::RasterizerState() : m_pRasterizerState(0)
{

}

RasterizerState::~RasterizerState()
{
	if (m_pRasterizerState)
	{
		m_pRasterizerState->Release();
		m_pRasterizerState = 0;
	}
}

void RasterizerState::Create(const RasterizerDesc& descRS, ID3D11Device* pDevice)
{
	D3D11_RASTERIZER_DESC desc;
	desc.DepthBias = descRS.depthBias;
	desc.DepthBiasClamp = descRS.depthBiasClamp;
	desc.FrontCounterClockwise = descRS.frontCounterClockwise;
	desc.MultisampleEnable = descRS.multisampleEnable;
	desc.ScissorEnable = descRS.scissorEnable;
	desc.SlopeScaledDepthBias = descRS.slopeScaleDepthBias;
	
	switch (descRS.fillMode)
	{
	case FILL_WIREFRAME:
		desc.FillMode = D3D11_FILL_WIREFRAME;
		break;
	case FILL_SOLID:
		desc.FillMode = D3D11_FILL_SOLID;
		break;
	}

	switch (descRS.cullMode)
	{
	case CULL_BACK:
		desc.CullMode = D3D11_CULL_BACK;
		break;
	case CULL_FRONT:
		desc.CullMode = D3D11_CULL_FRONT;
		break;
	case CULL_NONE:
		desc.CullMode = D3D11_CULL_NONE;
		break;
	}

	pDevice->CreateRasterizerState(&desc, &m_pRasterizerState);
}

//------------------------------------------------------------------
ID3D11RasterizerState* RasterizerState::GetD3DPtr()
{
	return m_pRasterizerState;
}

} // namespace HALgfx
} // namespace Magnet
