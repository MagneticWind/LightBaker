#ifndef TEXTURE2D_D3D11_H
#define TEXTURE2D_D3D11_H

#include "ITexture2d.h"
#include <d3d11.h>

namespace Magnet
{
namespace HALgfx
{

class Device;

class Texture2d : public ITexture2d
{
public:
	Texture2d();
	Texture2d(ID3D11Texture2D* pTexture2D);
	~Texture2d();

	void Create(const Texture2dDesc& texture2dDesc, const SubResourceData* subResourceData, ID3D11Device* pDevice);
	ID3D11Texture2D* GetD3DPtr();
	virtual ResourceType GetType() const;

private:
	ID3D11Texture2D* m_pTexture2D;
};

inline ID3D11Texture2D* Texture2d::GetD3DPtr()
{
	return m_pTexture2D;
}

inline ResourceType Texture2d::GetType() const
{
	return RESOURCE_TEXTURE2D;
}
} // namespace HALgfx
} // namespace Magnet
#endif