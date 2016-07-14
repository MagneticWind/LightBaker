#ifndef MATERIAL_SKY_H
#define MATERIAL_SKY_H

#include "IMaterial.h"
#include "Texture.h"

#include "Math\Vector4f.h"

namespace Magnet
{
namespace Scene
{
enum SkyType
{
	SKY_CUBE,
	SKY_PARABLOID
};

class MaterialSky : public IMaterial
{
public:
	MaterialSky(unsigned char tech) : IMaterial(tech)
	{
		m_pSkyTexture = 0;
	}
	virtual MaterialType GetType() const;

	SkyType GetSkyType() const;
	const Texture* GetTexture() const;
	void SetSkyType(SkyType skyType);
	void SetTexture(Texture* pTexture);

private:
	char m_caName[256];
	SkyType m_eType;

	Texture* m_pSkyTexture;
};

inline MaterialType MaterialSky::GetType() const
{
	return MATERIAL_SKY;
}

inline SkyType MaterialSky::GetSkyType() const
{
	return m_eType;
}

inline void MaterialSky::SetSkyType(SkyType skyType)
{
	m_eType = skyType;
}

inline void MaterialSky::SetTexture(Texture* pTexture)
{
	m_pSkyTexture = pTexture;
}

inline const Texture* MaterialSky::GetTexture() const
{
	return m_pSkyTexture;
}

} // namespace Scene
} // namespace Magnet

#endif