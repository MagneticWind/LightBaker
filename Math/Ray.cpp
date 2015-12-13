#include <math.h>
#include "Ray.h"

namespace Magnet
{
namespace Math
{
//------------------------------------------------------------------
float Ray::PointRayDistance(const Vector3f &v3Point) const
{
	Vector3f v3PO = v3Point - m_v3Origin;
	float fDot = Dot3f(v3PO, m_v3Direction);
	float fSquareLengthPO = v3PO.SquareLength();
	return sqrtf(fSquareLengthPO - fDot * fDot);
}
} // namespace Math
} // namespace Magnet