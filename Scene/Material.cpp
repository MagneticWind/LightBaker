#include "Material.h"

namespace Magnet
{
namespace Scene
{
//------------------------------------------------------------------
void Material::SetAmbient(const Math::Vector4f& v4Ambient)
{
	m_ka = v4Ambient;
}

//------------------------------------------------------------------
void Material::SetDiffuse(const Math::Vector4f& v4Diffuse)
{
	m_kd = v4Diffuse;
}

//------------------------------------------------------------------
void Material::SetSpecular(const Math::Vector4f& v4Specular)
{
	m_ks = v4Specular;
}

//------------------------------------------------------------------
void Material::SetEmission(const Math::Vector4f& v4Emission)
{
	m_ke = v4Emission;
}

//------------------------------------------------------------------
void Material::SetExponent(float fExponent)
{
	m_fExponent = fExponent;
}

} // namespace Scene
} // namespace Magnet
