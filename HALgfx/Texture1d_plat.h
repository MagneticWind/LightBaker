#ifndef TEXTURE1D_D3D11_H
#define TEXTURE1D_D3D11_H

#include "ITexture1d.h"
#include <d3d11.h>

namespace Magnet
{
namespace HALgfx
{

class Device;

class Texture1d : public ITexture1d
{
public:
	Texture1d();
	Texture1d(ID3D11Texture1D* pTexture1D);
	~Texture1d();

	void Create(const Texture1dDesc& texture1dDesc, const SubResourceData& subResourceData, ID3D11Device* pDevice);
	ID3D11Texture1D* GetD3DPtr();
	virtual ResourceType GetType() const;

private:
	ID3D11Texture1D* m_pTexture1D;
};

inline ID3D11Texture1D* Texture1d::GetD3DPtr()
{
	return m_pTexture1D;
}

inline ResourceType Texture1d::GetType() const
{
	return RESOURCE_TEXTURE1D;
}
} // namespace HALgfx
} // namespace Magnet
#endif