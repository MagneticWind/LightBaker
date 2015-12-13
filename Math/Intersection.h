#ifndef INTERSECTION_H
#define INTERSECTION_H


namespace Magnet
{
namespace Math
{
class Ray;
class Frustum;
class AABB;
class Sphere;
class Plane;
class Triangle;

class Intersection
{
public:
	static bool AABBInFrustum(const Frustum& frustum, const AABB& bbox);
	static bool SphereInFrustum(const Frustum& frustum, const Sphere& sphere);
	static bool PointInFrustum(const Vector3f& point, const Frustum& frustum);
	static bool RayAABB(const Ray& ray, const AABB& bbox);
	static bool RaySphere(const Ray& ray, const Sphere& sphere);
	static bool RayTriangle(const Ray& ray, const Triangle& triangle);
};
} // namespace Math
} // namespace Magnet

#endif