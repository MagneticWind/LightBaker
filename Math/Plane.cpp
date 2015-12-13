#include "Plane.h"

namespace Magnet
{
namespace Math
{
//------------------------------------------------------------------
Plane::Plane()
{
}

//------------------------------------------------------------------
Plane::Plane(const Vector3f& v3Point1, const Vector3f& v3Point2, const Vector3f& v3Point3)
{
	Vector3f v3P1P2 = v3Point2 - v3Point1;
	Vector3f v3P1P3 = v3Point3 - v3Point1;
	m_v3Normal = Cross3f(v3P1P2, v3P1P3);
	m_v3Normal.Normalize();
	m_v3Point = v3Point1;
	m_fD = -Dot3f(m_v3Normal, m_v3Point);
}

//------------------------------------------------------------------
Plane::Plane(const Vector3f& v3Point, const Vector3f& v3Normal) : m_v3Point(v3Point), m_v3Normal(v3Normal)
{
	m_fD = -Dot3f(m_v3Normal, m_v3Point);
}

//------------------------------------------------------------------
Plane::Plane(const float fA, const float fB, const float fC, const float fD)
{
	m_v3Normal = Vector3f(fA, fB, fC);
	m_fD = fD / m_v3Normal.Length(); //@TODO verify this.
	m_v3Normal.Normalize();
}

//------------------------------------------------------------------
void Plane::SetBy3Points(const Vector3f& v3Point1, const Vector3f& v3Point2, const Vector3f& v3Point3)
{
	Vector3f v3P1P2 = v3Point2 - v3Point1;
	Vector3f v3P1P3 = v3Point3 - v3Point1;
	m_v3Normal = Cross3f(v3P1P2, v3P1P3);
	m_v3Normal.Normalize();
	m_v3Point = v3Point1;
	m_fD = -Dot3f(m_v3Normal, m_v3Point);
}

//------------------------------------------------------------------
void Plane::SetByPositionAndNormal(const Vector3f& v3Point, const Vector3f& v3Normal)
{
	m_v3Point = v3Point;
	m_v3Normal = v3Normal;
	m_fD = -Dot3f(m_v3Normal, m_v3Point);
}

//------------------------------------------------------------------
float Plane::PointPlaneDistance(const Vector3f &v3Point) const
{
	return Dot3f(v3Point, m_v3Normal) + m_fD;
}
} // namespace Math
} // namespace Magnet
