#include "Sphere.h"

namespace Magnet
{
Sphere::Sphere()
{
}

Sphere::Sphere(const Vector3f& v3Center, float fRadius) : m_v3Center(v3Center), m_fRadius(fRadius)
{
}

} // namespace Magnet