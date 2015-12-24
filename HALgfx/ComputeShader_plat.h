#ifndef COMPUTE_SHADER_H
#define COMPUTE_SHADER_H

#include <d3d11.h>
#include "IShader.h"

namespace Magnet
{
namespace HALgfx
{
class ComputeShader : public IShader
{
public:
	ComputeShader();
	~ComputeShader();
	void Create(const void* pShaderSource, int iSize, ID3D11Device* pDevice);
	ID3D11ComputeShader* GetD3DPtr();

private:
	ID3D11ComputeShader* m_pComputeShader;
};
} // namespace HALgfx
} // namespace Magnet


#endif