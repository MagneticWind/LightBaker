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

public:
	Vector3f m_v3Center;
	float m_fRadius;
};
} // namespace Math
} // namespace Magnet

#endif