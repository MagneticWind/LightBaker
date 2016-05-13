#ifndef SPHERE_H
#define SPHERE_H

#include "Vector3f.h"

namespace Magnet
{
namespace Math
{
class Sphere
{
public:
	Sphere(){}
	Sphere(const Vector3f& v3Center, float fRadius) : m_v3Center(v3Center), m_fRadius(fRadius)
	{
	}
	static Sphere CombineSphere(const Sphere& sphere1, const Sphere& sphere2);

public:
	Vector3f m_v3Center;
	float m_fRadius;
};

inline Sphere Sphere::CombineSphere(const Sphere& sphere1, const Sphere& sphere2)
{
	Vector3f v3CenterDir = sphere1.m_v3Center - sphere2.m_v3Center;
	const float fCenterDistance = v3CenterDir.Length();
	v3CenterDir.Normalize();

	if (sphere1.m_fRadius >= sphere2.m_fRadius)
	{
		if (fCenterDistance <= (sphere1.m_fRadius - sphere2.m_fRadius))
		{
			return Sphere(sphere1.m_v3Center, sphere1.m_fRadius);
		}
		else
		{
			const float fNewRadius = (sphere1.m_fRadius + sphere2.m_fRadius + fCenterDistance) * 0.5f;
			const Vector3f v3NewCenter = sphere1.m_v3Center + v3CenterDir * (fNewRadius - sphere1.m_fRadius);
			return Sphere(v3NewCenter, fNewRadius);
		}
	}
	else
	{
		if (fCenterDistance <= (sphere2.m_fRadius - sphere1.m_fRadius))
		{
			return Sphere(sphere2.m_v3Center, sphere2.m_fRadius);
		}
		else
		{
			const float fNewRadius = (sphere1.m_fRadius + sphere2.m_fRadius + fCenterDistance) * 0.5f;
			const Vector3f v3NewCenter = sphere1.m_v3Center + v3CenterDir * (fNewRadius - sphere1.m_fRadius);
			return Sphere(v3NewCenter, fNewRadius);
		}
	}
}

} // namespace Math
} // namespace Magnet

#endif