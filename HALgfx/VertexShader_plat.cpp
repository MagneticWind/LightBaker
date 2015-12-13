#include "VertexShader_plat.h"

namespace Magnet
{
namespace HALgfx
{
//------------------------------------------------------------------
VertexShader::VertexShader() : m_pVertexShader(0)
{
}

//------------------------------------------------------------------
VertexShader::~VertexShader()
{
	if (m_pVertexShader)
	{
		m_pVertexShader->Release();
		m_pVertexShader = 0;
	}
}

//------------------------------------------------------------------
void VertexShader::Create(const void* pShaderSource, int iSize, ID3D11Device* pDevice)
{
	pDevice->CreateVertexShader( pShaderSource, iSize, NULL, &m_pVertexShader);
}

//------------------------------------------------------------------
ID3D11VertexShader* VertexShader::GetD3DPtr()
{
	return m_pVertexShader;
}

} // namespace HALgfx
} // namespace Magnet