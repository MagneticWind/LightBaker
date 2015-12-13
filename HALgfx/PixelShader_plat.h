#ifndef PIXEL_SHADER_H
#define PIXEL_SHADER_H

#include <d3d11.h>
#include "IShader.h"

namespace Magnet
{
namespace HALgfx
{
class PixelShader : public IShader
{
public:
	PixelShader();
	~PixelShader();
	void Create(const void* pShaderSource, int iSize, ID3D11Device* pDevice);

	ID3D11PixelShader* GetD3DPtr();

private:
	ID3D11PixelShader* m_pPixelShader;
};
} // namespace HALgfx
} // namespace Magnet


#endif