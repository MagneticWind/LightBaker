#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "Vector3f.h"
#include "Plane.h"

namespace Magnet
{
namespace Math
{
class Frustum
{
public:

	// got compiling errors for the enum
	/*
	enum Planes
	{
		TOP = 0,
		BOTTOM = 1,
		LEFT = 2,
		RIGHT = 3,
		NEAR = 4,
		FAR = 5
	};
	*/

	Frustum(){}
	Frustum(const Vector3f* pCornerPoints)
	{
		for (int i = 0; i < 8; ++i)
			m_CornerPoints[i] = pCornerPoints[i];
	}
	Frustum& operator=(const Frustum& frustum)
	{
		for (int i = 0; i < 6; ++i)
		{
			m_Planes[i] = frustum.m_Planes[i];
		}
		for (int i = 0; i < 8; ++i)
		{
			m_CornerPoints[i] = frustum.m_CornerPoints[i];
		}
	}

public:
	Vector3f GetCenter() const;

public:
	Plane m_Planes[6];
	Vector3f m_CornerPoints[8];
};
} // namespace Math
} // namepace Magnet

#endif