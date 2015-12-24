#include "ComputeShader_plat.h"

namespace Magnet
{
namespace HALgfx
{
//------------------------------------------------------------------
ComputeShader::ComputeShader() : m_pComputeShader(0)
{
}

//------------------------------------------------------------------
ComputeShader::~ComputeShader()
{
	if (m_pComputeShader)
	{
		m_pComputeShader->Release();
		m_pComputeShader = 0;
	}
}

//------------------------------------------------------------------
void ComputeShader::Create(const void* pShaderSource, int iSize, ID3D11Device* pDevice)
{
	pDevice->CreateComputeShader(pShaderSource, iSize, NULL, &m_pComputeShader);
}

//------------------------------------------------------------------
ID3D11ComputeShader* ComputeShader::GetD3DPtr()
{
	return m_pComputeShader;
}

} // namespace HALgfx
} // namespace Magnet