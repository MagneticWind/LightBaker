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
	const Vector3f* GetCornerPoints() const;
	void Update(const Vector3f& v3Position, const Vector3f& v3View, const Vector3f& v3Left, const Vector3f& v3Up, float fNear, float fFar, float fFov, float fAspectRatio);

public:
	Plane m_Planes[6];
	Vector3f m_CornerPoints[8];

};

inline const Vector3f* Frustum::GetCornerPoints() const
{
	return m_CornerPoints;
}

inline void Frustum::Update(const Vector3f& v3Position, const Vector3f& v3View, const Vector3f& v3Left, const Vector3f& v3Up, float fNear, float fFar, float fFov, float fAspectRatio)
{
	Vector3f v3NearCenter = v3Position + v3View * fNear;
	Vector3f v3FarCenter = v3Position + v3View * fFar;

	float fNearHeight = 2.f * tanf(fFov * M_PI / 180.f * 0.5f) * fNear;
	float fFarHeight = 2.f * tanf(fFov * M_PI / 180.f* 0.5f) * fFar;
	float fNearWidth = fNearHeight * fAspectRatio;
	float fFarWidth = fFarHeight * fAspectRatio;

	// @NOTE: more efficient method exist
	Vector3f v3FarTopLeft = v3FarCenter + v3Up * fFarHeight * 0.5f + v3Left * fFarWidth * 0.5f;
	Vector3f v3FarTopRight = v3FarCenter + v3Up * fFarHeight * 0.5f - v3Left * fFarWidth * 0.5f;
	Vector3f v3FarBottomLeft = v3FarCenter - v3Up * fFarHeight * 0.5f + v3Left * fFarWidth * 0.5f;
	Vector3f v3FarBottomRight = v3FarCenter - v3Up * fFarHeight * 0.5f - v3Left * fFarWidth * 0.5f;

	Vector3f v3NearTopLeft = v3NearCenter + v3Up * fNearHeight * 0.5f + v3Left * fNearWidth * 0.5f;
	Vector3f v3NearTopRight = v3NearCenter + v3Up * fNearHeight * 0.5f - v3Left * fNearWidth * 0.5f;
	Vector3f v3NearBottomLeft = v3NearCenter - v3Up * fNearHeight * 0.5f + v3Left * fNearWidth * 0.5f;
	Vector3f v3NearBottomRight = v3NearCenter - v3Up * fNearHeight * 0.5f - v3Left * fNearWidth * 0.5f;

	m_CornerPoints[0] = v3NearTopLeft;
	m_CornerPoints[1] = v3NearBottomLeft;
	m_CornerPoints[2] = v3NearBottomRight;
	m_CornerPoints[3] = v3NearTopRight;
	m_CornerPoints[4] = v3FarTopLeft;
	m_CornerPoints[5] = v3FarBottomLeft;
	m_CornerPoints[6] = v3FarBottomRight;
	m_CornerPoints[7] = v3FarTopRight;

	//@NOTE: check Frustum.h for order or planes, can't use enum in Frustum, don't know why
	/*m_frustum.m_Planes[0].SetBy3Points(v3FarTopLeft, v3NearTopLeft, v3NearTopRight);
	m_frustum.m_Planes[1].SetBy3Points(v3NearBottomLeft, v3FarBottomLeft, v3FarBottomRight);
	m_frustum.m_Planes[2].SetBy3Points(v3NearBottomLeft, v3NearTopLeft, v3FarTopLeft);
	m_frustum.m_Planes[3].SetBy3Points(v3NearTopRight, v3NearBottomRight, v3FarBottomRight);
	m_frustum.m_Planes[4].SetBy3Points(v3NearTopLeft, v3NearBottomLeft, v3NearBottomRight);
	m_frustum.m_Planes[5].SetBy3Points(v3FarTopLeft, v3FarTopRight, v3FarBottomRight);*/

	m_Planes[0].SetBy3Points(v3FarTopLeft, v3NearTopRight, v3NearTopLeft);
	m_Planes[1].SetBy3Points(v3NearBottomLeft, v3FarBottomRight, v3FarBottomLeft);
	m_Planes[2].SetBy3Points(v3NearBottomLeft, v3FarTopLeft, v3NearTopLeft);
	m_Planes[3].SetBy3Points(v3NearTopRight, v3FarBottomRight, v3NearBottomRight);
	m_Planes[4].SetBy3Points(v3NearTopLeft, v3NearBottomRight, v3NearBottomLeft);
	m_Planes[5].SetBy3Points(v3FarTopLeft, v3FarBottomRight, v3FarTopRight);
}

} // namespace Math
} // namepace Magnet

#endif