#ifndef RAY_H
#define RAY_H

#include "Vector3f.h"

namespace Magnet
{
namespace Math
{
class Ray
{
public:
	Ray();
	Ray(const Vector3f& o, const Vector3f& d);
	inline Vector3f PointAtParameter(const float t) const;
	float PointRayDistance(const Vector3f& v3Point) const;

public:
	Vector3f m_v3Origin;
	Vector3f m_v3Direction;
};

//------------------------------------------------------------------
inline Ray::Ray() : m_v3Origin(0.0f), m_v3Direction(0.0f, 0.0f, 1.0f) {}

//------------------------------------------------------------------
inline Ray::Ray(const Vector3f& o, const Vector3f& r) : m_v3Origin(o), m_v3Direction(r)
{
	m_v3Direction.Normalize();
}

//------------------------------------------------------------------
inline Vector3f Ray::PointAtParameter(const float t) const
{
	return m_v3Origin + m_v3Direction * t;
}
} // namespace Math
} // namespace Magnet

#endif
