
#include "AABB.h"

namespace Magnet
{
namespace Math
{
//------------------------------------------------------------------
AABB::AABB() : m_v3Min(10000.0f), m_v3Max(-10000.0f){}

//------------------------------------------------------------------
AABB::AABB(const Vector3f &v3Min, const Vector3f &v3Max) : m_v3Min(v3Min), m_v3Max(v3Max) {}

//------------------------------------------------------------------
AABB AABB::CombineBBox(const AABB &bbox1, const AABB &bbox2)
{
	Vector3f v3Min;
	Vector3f v3Max;

	// the reason to use const reference here is to eliminate the temp object,
	// as well as decrease the getter functions calls
	const Vector3f& v3BBox1Min = bbox1.GetMinPoint();
	const Vector3f& v3BBox1Max = bbox1.GetMaxPoint();
	const Vector3f& v3BBox2Min = bbox2.GetMinPoint();
	const Vector3f& v3BBox2Max = bbox2.GetMaxPoint();

	v3Min.x = min(v3BBox1Min.x, v3BBox2Min.x);
	v3Min.y = min(v3BBox1Min.y, v3BBox2Min.y);
	v3Min.z = min(v3BBox1Min.z, v3BBox2Min.z);

	v3Max.x = max(v3BBox1Max.x, v3BBox2Max.x);
	v3Max.y = max(v3BBox1Max.y, v3BBox2Max.y);
	v3Max.z = max(v3BBox1Max.z, v3BBox2Max.z);

	return AABB(v3Min, v3Max);
}
} // namespace Math
} // namespace Magnet