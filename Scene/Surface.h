#ifndef SURFACE_H
#define SURFACE_H

#include "Math\AABB.h"
#include "Math\Sphere.h"
#include "Math\Matrix4f.h"
#include "IGeometry.h"
#include "IMaterial.h"
#include "Texture.h"

namespace Magnet
{
namespace Scene
{

class Surface
{
public:
	Surface()
	{
	}
	~Surface();

	const Math::AABB& GetBBox() const;
	const Math::Sphere& GetBSphere() const;
	const IMaterial* GetMaterial() const;
	const IGeometry* GetGeometry() const;

	void SetMaterial(IMaterial* pMaterial);
	void SetGeometry(IGeometry* pGeometry);

private:
	IGeometry* m_pGeometry;
	IMaterial* m_pMaterial;

	// AABB and Sphere here is at object space or original world space when loading the model
	// Not considering transformations in world space when updating
	Math::AABB m_BBox;
	Math::Sphere m_BSphere;
};

//------------------------------------------------------------------
inline const Math::AABB& Surface::GetBBox() const
{
	return m_BBox;
}

//------------------------------------------------------------------
inline const Math::Sphere& Surface::GetBSphere() const
{
	return m_BSphere;
}

//------------------------------------------------------------------
inline const IMaterial* Surface::GetMaterial() const
{
	return m_pMaterial;
}

//------------------------------------------------------------------
inline const IGeometry* Surface::GetGeometry() const
{
	return m_pGeometry;
}

//------------------------------------------------------------------
inline void Surface::SetMaterial(IMaterial* pMaterial)
{
	m_pMaterial = pMaterial;
}

//------------------------------------------------------------------
inline void Surface::SetGeometry(IGeometry* pGeometry)
{
	m_pGeometry = pGeometry;
}

} // namespace Scene
} // namespace Magnet

#endif