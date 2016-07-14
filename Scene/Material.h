#ifndef MATERIAL_H
#define MATERIAL_H

#include "IMaterial.h"
#include "Texture.h"

#include "Math\Vector4f.h"

#define MAX_NUM_TEXTURES 8

namespace Magnet
{
namespace Scene
{

class Material : public IMaterial
{
public:
	Material(unsigned char tech) : IMaterial(tech)
	{
		m_iNumTextures = 0;
	}
	virtual MaterialType GetType() const;

	void SetAmbient(const Math::Vector4f& v4Ambient);
	void SetDiffuse(const Math::Vector4f& v4Diffuse);
	void SetSpecular(const Math::Vector4f& v4Specular);
	void SetEmission(const Math::Vector4f& v4Emission);
	void SetExponent(float fExponent);

	void GetAmbient(Math::Vector4f& v4Out) const;
	void GetDiffuse(Math::Vector4f& v4Out) const;
	void GetSpecular(Math::Vector4f& v4Out) const;
	void GetEmission(Math::Vector4f& v4Out) const;
	void GetExponent(float& fOut) const;

	const Texture* GetTexture(int iIndex) const;
	void AddTexture(Texture* pTexture);
	int GetNumTextures() const;

private:
	char m_caName[256];

	Texture* m_ppTextures[MAX_NUM_TEXTURES];
	int m_iNumTextures;

	Math::Vector4f m_ka;    // ambient
	Math::Vector4f m_kd;    // diffuse
	Math::Vector4f m_ks;    // specular
	Math::Vector4f m_ke;    // emission
	float m_fExponent;
};

inline MaterialType Material::GetType() const
{
	return MATERIAL_NORMAL;
}

inline void Material::GetAmbient(Math::Vector4f& v4Out) const
{
	v4Out = m_ka;
}

inline void Material::GetDiffuse(Math::Vector4f& v4Out) const
{
	v4Out = m_kd;
}

inline void Material::GetSpecular(Math::Vector4f& v4Out) const
{
	v4Out = m_ks;
}

inline void Material::GetEmission(Math::Vector4f& v4Out) const
{
	v4Out = m_ke;
}

inline void Material::GetExponent(float& fOut) const
{
	fOut = m_fExponent;
}

//------------------------------------------------------------------
inline void Material::AddTexture(Texture* pTexture)
{
	m_ppTextures[m_iNumTextures++] = pTexture;
}

//------------------------------------------------------------------
inline int Material::GetNumTextures() const
{
	return m_iNumTextures;
}

//------------------------------------------------------------------
inline const Texture* Material::GetTexture(int iIndex) const
{
	return m_ppTextures[iIndex];
}

} // namespace Scene
} // namespace Magnet

#endif