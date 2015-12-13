#ifndef VERTEX_SHADER_H
#define VERTEX_SHADER_H

#include <d3d11.h>
#include "IShader.h"

namespace Magnet
{
namespace HALgfx
{
class VertexShader : public IShader
{
public:
	VertexShader();
	~VertexShader();
	void Create(const void* pShaderSource, int iSize, ID3D11Device* pDevice);
	ID3D11VertexShader* GetD3DPtr();

private:
	ID3D11VertexShader* m_pVertexShader;
};
} // namespace HALgfx
} // namespace Magnet


#endif