#include "PixelShader_plat.h"

namespace Magnet
{
namespace HALgfx
{
//------------------------------------------------------------------
PixelShader::PixelShader() : m_pPixelShader(0)
{
}

//------------------------------------------------------------------
PixelShader::~PixelShader()
{
	if (m_pPixelShader)
	{
		m_pPixelShader->Release();
		m_pPixelShader = 0;
	}
}

//------------------------------------------------------------------
void PixelShader::Create(const void* pShaderSource, int iSize, ID3D11Device* pDevice)
{
	pDevice->CreatePixelShader( pShaderSource, iSize, NULL, &m_pPixelShader);
}

//------------------------------------------------------------------
ID3D11PixelShader* PixelShader::GetD3DPtr()
{
	return m_pPixelShader;
}

} // namespace HALgfx
} // namespace Magnet