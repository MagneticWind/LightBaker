#ifndef AABB_H
#define AABB_H

#include "Macros.h"
#include "Vector3f.h"

namespace Magnet
{
namespace Math
{

class AABB
{
public:
	AABB();
	AABB(const Vector3f& v3Min, const Vector3f& v3Max);

	const Vector3f& GetMinPoint() const;
	const Vector3f& GetMaxPoint() const;
	float GetRadius() const;
	Vector3f GetCenter() const;

	Vector3f ComputePositiveVertex(const Vector3f& v3Normal) const;
	Vector3f ComputeNegativeVertex(const Vector3f& v3Normal) const;

	static AABB CombineBBox(const AABB& bbox1, const AABB& bbox2);

	void Update(const Vector3f& v3Point);

private:
	Vector3f m_v3Min;
	Vector3f m_v3Max;
};

//------------------------------------------------------------------
inline const Vector3f& AABB::GetMinPoint() const
{
	return m_v3Min;
}

//------------------------------------------------------------------
inline const Vector3f& AABB::GetMaxPoint() const
{
	return m_v3Max;
}

//------------------------------------------------------------------
inline float AABB::GetRadius() const
{
	return (m_v3Max - m_v3Min).Length() * 0.5f;
}

//------------------------------------------------------------------
inline Vector3f AABB::GetCenter() const
{
	return (m_v3Max + m_v3Min) * 0.5f;
}

//------------------------------------------------------------------
inline Vector3f AABB::ComputePositiveVertex(const Vector3f& v3Normal) const
{
	Vector3f v3Vertex = m_v3Min;
	if (v3Normal.x >= 0.f)
		v3Vertex.x = m_v3Max.x;
	if (v3Normal.y >= 0.f)
		v3Vertex.y = m_v3Max.y;
	if (v3Normal.z >= 0.f)
		v3Vertex.z = m_v3Max.z;
	return v3Vertex;

}

//------------------------------------------------------------------
inline Vector3f AABB::ComputeNegativeVertex(const Vector3f& v3Normal) const
{
	Vector3f v3Vertex = m_v3Max;
	if (v3Normal.x >= 0.f)
		v3Vertex.x = m_v3Min.x;
	if (v3Normal.y >= 0.f)
		v3Vertex.y = m_v3Min.y;
	if (v3Normal.z >= 0.f)
		v3Vertex.z = m_v3Min.z;
	return v3Vertex;
}

//------------------------------------------------------------------
inline void AABB::Update(const Vector3f& v3Point)
{
	m_v3Min.x = min(m_v3Min.x, v3Point.x);
	m_v3Min.y = min(m_v3Min.y, v3Point.y);
	m_v3Min.z = min(m_v3Min.z, v3Point.z);

	m_v3Max.x = max(m_v3Max.x, v3Point.x);
	m_v3Max.y = max(m_v3Max.y, v3Point.y);
	m_v3Max.z = max(m_v3Max.z, v3Point.z);
}
} // namespace Math
} // namespace Magnet

#endif
