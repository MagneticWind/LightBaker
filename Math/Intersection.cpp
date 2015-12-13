#include "Vector3f.h"
#include "Ray.h"
#include "Plane.h"
#include "Frustum.h"
#include "Sphere.h"
#include "AABB.h"

#include "Intersection.h"

namespace Magnet
{
namespace Math
{
//------------------------------------------------------------------
bool Intersection::AABBInFrustum(const Frustum& frustum, const AABB& bbox)
{
	for (int i = 0; i < 6; ++i)
	{
		const Vector3f v3Positive = bbox.ComputePositiveVertex(frustum.m_Planes[i].m_v3Normal);
		const Vector3f v3Negative = bbox.ComputeNegativeVertex(frustum.m_Planes[i].m_v3Normal);
		if (frustum.m_Planes[i].PointPlaneDistance(v3Positive) < 0)
			return false;
		else if (frustum.m_Planes[i].PointPlaneDistance(v3Negative) < 0)
			return true;
	}
	return true;
}

//------------------------------------------------------------------
bool Intersection::SphereInFrustum(const Frustum& frustum, const Sphere& sphere)
{
	for (int i = 0; i < 6; ++i)
	{
		const float fDistance = frustum.m_Planes[i].PointPlaneDistance(sphere.m_v3Center);
		if (fDistance - sphere.m_fRadius < 0.f)
			return true;
	}
	return false;
}

//------------------------------------------------------------------
bool Intersection::PointInFrustum(const Vector3f& point, const Frustum& frustum)
{
	for (int i = 0; i < 6; ++i)
	{
		const float fDistance = frustum.m_Planes[i].PointPlaneDistance(point);
		if (fDistance < 0.f)
			return false;
	}
	return true;
}

//------------------------------------------------------------------
bool Intersection::RayAABB(const Ray& ray, const AABB& bbox)
{
	return true;
}

//------------------------------------------------------------------
bool Intersection::RaySphere(const Ray& ray, const Sphere& sphere)
{
	return true;
}

//------------------------------------------------------------------
bool Intersection::RayTriangle(const Ray& ray, const Triangle& triangle)
{
	return true;
}
} // namespace Math
} // namespace Magnet