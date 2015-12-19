#ifndef MATERIAL_SKY_H
#define MATERIAL_SKY_H

#include "IMaterial.h"

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
	{}
	virtual MaterialType GetType() const;

	SkyType GetSkyType() const;
	const char* GetMapName() const;
	void SetSkyType(SkyType skyType);
	void SetTextureName(const char* texturePath);

private:
	char m_caName[256];
	SkyType m_eType;
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

inline void MaterialSky::SetTextureName(const char* texturePath)
{
	strcpy(m_caName, texturePath);
}

inline const char* MaterialSky::GetMapName() const
{
	return m_caName;
}

} // namespace Scene
} // namespace Magnet

#endif