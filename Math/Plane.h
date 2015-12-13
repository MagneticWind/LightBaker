#ifndef PLANE_H
#define PLANE_H

#include "Vector3f.h"

namespace Magnet
{
namespace Math
{
class Plane
{
public:
	Plane();
	Plane(const Vector3f& v3Point1, const Vector3f& v3Point2, const Vector3f& v3Point3);
	Plane(const Vector3f& v3Point, const Vector3f& v3Normal);
	Plane(float fA, float fB, float fC, float fD);

	void SetBy3Points(const Vector3f& v3Point1, const Vector3f& v3Point2, const Vector3f& v3Point3);
	void SetByPositionAndNormal(const Vector3f& v3Point, const Vector3f& v3Normal);

	float PointPlaneDistance(const Vector3f &v3Point) const;

public:
	Vector3f m_v3Point;
	Vector3f m_v3Normal;
	float    m_fD; // parameter D of Ax + By + Cz + D = 0;
};
} // namespace Math
} // namespace Magnet
#endif