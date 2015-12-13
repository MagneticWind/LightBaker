#ifndef IMATERIAL_H
#define IMATERIAL_H

#include <string.h>

namespace Magnet
{
namespace Scene
{
enum MaterialType
{
	MATERIAL_NORMAL,
	MATERIAL_TRANSPARENT,
	MAX_TYPE_COUNT
};

enum MaterialTech
{
	COLOR_MAP       = 1,
	NORMAL_MAP      = 2,
	LIGHT_MAP       = 4,
	SHADOW_CASTER   = 8,
	SHADOW_RECEIVER = 16,
	PROBE_VOLUME    = 32,
	SKY_SH          = 64
};

class IMaterial
{
public:
	IMaterial(unsigned char tech) : 
		m_tech(tech)
	{}
	virtual MaterialType GetType() const = 0;

	unsigned char GetTech() const { return m_tech; }

	const void GetTechString(char* pTechString) const
	{
		strcpy(pTechString, "");

		if (m_tech & COLOR_MAP)
		{
			strcat(pTechString, "_c");
		}
		if (m_tech & NORMAL_MAP)
		{
			strcat(pTechString, "_n");
		}
		if (m_tech & LIGHT_MAP)
		{
			strcat(pTechString, "_l");
		}
		if (m_tech & SHADOW_CASTER)
		{
			strcat(pTechString, "_sc");
		}
		if (m_tech & SHADOW_RECEIVER)
		{
			strcat(pTechString, "_sr");
		}
		if (m_tech & PROBE_VOLUME)
		{
			strcat(pTechString, "_p");
		}
		if (m_tech & SKY_SH)
		{
			strcat(pTechString, "sh");
		}

	}
private:
	unsigned char m_tech;
};
} // namespace Scene
} // namespace Magnet
#endif