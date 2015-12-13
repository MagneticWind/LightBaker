#include "BlendState_plat.h"

namespace Magnet
{
namespace HALgfx
{
//------------------------------------------------------------------
BlendState::BlendState(const BlendDesc& blendDesc) : m_desc(blendDesc), m_pBlendState(0)
{
}

//------------------------------------------------------------------
BlendState::~BlendState()
{
	if (m_pBlendState)
	{
		m_pBlendState->Release();
		m_pBlendState = 0;
	}
}

//------------------------------------------------------------------
D3D11_BLEND BlendState::GetD3DBlend(Blend blend)
{
	switch(blend)
	{
	case BLEND_ZERO:
		return D3D11_BLEND_ZERO;
	case BLEND_ONE:
		return D3D11_BLEND_ONE;
	}
}

//------------------------------------------------------------------
D3D11_BLEND_OP BlendState::GetD3DBlendOp(BlendOp blendOp)
{
	switch(blendOp)
	{
	case BLEND_OP_ADD:
		return D3D11_BLEND_OP_ADD;
	case BLEND_OP_SUBTRACT:
		return D3D11_BLEND_OP_SUBTRACT;
	case BLEND_OP_REV_SUBTRACT:
		return D3D11_BLEND_OP_REV_SUBTRACT;
	case BLEND_OP_MIN:
		return D3D11_BLEND_OP_MIN;
	case BLEND_OP_MAX:
		return D3D11_BLEND_OP_MAX;
	}
}

//------------------------------------------------------------------
void BlendState::Create(ID3D11Device* pDevice)
{
	D3D11_BLEND_DESC desc;
	desc.AlphaToCoverageEnable = m_desc.alphaToCoverageEnable;
	desc.IndependentBlendEnable = m_desc.independentBlendEnable;
	
	if(desc.IndependentBlendEnable)
	{
		for (int i = 0; i < 8; ++i)
		{
			desc.RenderTarget[i].BlendEnable = m_desc.renderTarget[i].blendEnable;

			desc.RenderTarget[i].SrcBlend = GetD3DBlend(m_desc.renderTarget[i].srcBlend);
			desc.RenderTarget[i].BlendOp = GetD3DBlendOp(m_desc.renderTarget[i].blendOp);
			desc.RenderTarget[i].DestBlend = GetD3DBlend(m_desc.renderTarget[i].dstBlend);

			desc.RenderTarget[i].SrcBlendAlpha = GetD3DBlend(m_desc.renderTarget[i].scrBlendAlpha);
			desc.RenderTarget[i].BlendOpAlpha = GetD3DBlendOp(m_desc.renderTarget[i].BlendOpAlpha);
			desc.RenderTarget[i].DestBlendAlpha = GetD3DBlend(m_desc.renderTarget[i].dstBlendAlpha);

		}
	}
	else
	{
		desc.RenderTarget[0].BlendEnable = m_desc.renderTarget[0].blendEnable;

		desc.RenderTarget[0].SrcBlend = GetD3DBlend(m_desc.renderTarget[0].srcBlend);
		desc.RenderTarget[0].BlendOp = GetD3DBlendOp(m_desc.renderTarget[0].blendOp);
		desc.RenderTarget[0].DestBlend = GetD3DBlend(m_desc.renderTarget[0].dstBlend);

		desc.RenderTarget[0].SrcBlendAlpha = GetD3DBlend(m_desc.renderTarget[0].scrBlendAlpha);
		desc.RenderTarget[0].BlendOpAlpha = GetD3DBlendOp(m_desc.renderTarget[0].BlendOpAlpha);
		desc.RenderTarget[0].DestBlendAlpha = GetD3DBlend(m_desc.renderTarget[0].dstBlendAlpha);
	}

	pDevice->CreateBlendState(&desc, &m_pBlendState);

}

} // namespace HALgfx
} // namespace Magnet