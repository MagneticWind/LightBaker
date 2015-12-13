#include "DepthStencilState_plat.h"
#include "ComparisonFunc_plat.h"

namespace Magnet
{
namespace HALgfx
{
//------------------------------------------------------------------
DepthStencilState::DepthStencilState(const DepthStencilDesc& descDS) : m_desc(descDS), m_pDepthStencilState(0)
{
}

//------------------------------------------------------------------
DepthStencilState::~DepthStencilState()
{
	if (m_pDepthStencilState)
	{
		m_pDepthStencilState->Release();
		m_pDepthStencilState = 0;
	}
}

//------------------------------------------------------------------
D3D11_STENCIL_OP DepthStencilState::GetD3DStencilOp(StencilOP op)
{
	switch(op)
	{
	case STENCIL_OP_KEEP:
		return D3D11_STENCIL_OP_KEEP;
	case STENCIL_OP_ZERO:
		return D3D11_STENCIL_OP_ZERO;
	}
}

//------------------------------------------------------------------
void DepthStencilState::Create(ID3D11Device* pDevice)
{
	D3D11_DEPTH_STENCIL_DESC desc;
	desc.DepthEnable = m_desc.depthEnable;
	desc.DepthFunc = GetD3DComparisonFunc(m_desc.depthFunc);
	desc.StencilEnable = m_desc.stencilEnable;
	if (m_desc.depthWriteMask == DEPTH_WRITE_MASK_ZERO)
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	else if (m_desc.depthWriteMask == D3D11_DEPTH_WRITE_MASK_ALL)
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.StencilReadMask = m_desc.stencilReadMask;
	desc.StencilWriteMask = m_desc.stencilWriteMask;
	
	desc.FrontFace.StencilFailOp = GetD3DStencilOp(m_desc.frontFace.stencilFailOp);
	desc.FrontFace.StencilPassOp = GetD3DStencilOp(m_desc.frontFace.stencilPassOp);
	desc.FrontFace.StencilDepthFailOp = GetD3DStencilOp(m_desc.frontFace.stencilDepthFailOp);
	desc.FrontFace.StencilFunc = GetD3DComparisonFunc(m_desc.frontFace.stencilFunc);

	desc.BackFace.StencilFailOp = GetD3DStencilOp(m_desc.backFace.stencilFailOp);
	desc.BackFace.StencilPassOp = GetD3DStencilOp(m_desc.backFace.stencilPassOp);
	desc.BackFace.StencilDepthFailOp = GetD3DStencilOp(m_desc.backFace.stencilDepthFailOp);
	desc.BackFace.StencilFunc = GetD3DComparisonFunc(m_desc.backFace.stencilFunc);

	pDevice->CreateDepthStencilState(&desc, &m_pDepthStencilState);
}

} // namespace HALgfx
} // namespace Magnet