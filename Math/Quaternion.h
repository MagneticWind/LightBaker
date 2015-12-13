#ifndef QUATERNION_H
#define QUATERNION_H

#include "Matrix4f.h"

namespace Magnet
{
namespace Math
{

class Quaternion
{
public:
	Quaternion(); // default constructor
	Quaternion(float fx, float fy, float fz, float fw); // constructor
	Quaternion(const Quaternion& q); // copy constructor
	Quaternion(float f);

	Quaternion& operator = (const Quaternion& q); // assignment operator

	Quaternion operator - () const;

	Quaternion& operator += (const Quaternion& q);
	Quaternion& operator -= (const Quaternion& q);
	Quaternion& operator *= (const Quaternion& q);
	Quaternion& operator *= (float f);
	Quaternion& operator /= (float f);

	Quaternion operator + (const Quaternion& q) const;
	Quaternion operator - (const Quaternion& q) const;
	Quaternion operator * (const Quaternion& q) const;
	Quaternion operator * (float f) const;
	Quaternion operator / (float f) const;

	float x, y, z, w;
};

inline Quaternion::Quaternion() : x(0.f), y(0.f), z(0.f), w(0.f)
{
}

inline Quaternion::Quaternion(float fx, float fy, float fz, float fw) : x(fx), y(fy), z(fz), w(fw)
{
}

inline Quaternion::Quaternion(const Quaternion& q) : x(q.x), y(q.y), z(q.z), w(q.w)
{
}

inline Quaternion::Quaternion(float f) : x(f), y(f), z(f), w(f)
{
}

inline Quaternion& Quaternion::operator = (const Quaternion& q)
{
	x = q.x;
	y = q.y;
	z = q.z;
	w = q.w;
	return *this;
}

inline Quaternion Quaternion::operator - () const
{
	return Quaternion(-x, -y, -z, -w);
}

inline Quaternion& Quaternion::operator += (const Quaternion& q)
{
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
	return *this;
}

inline Quaternion& Quaternion::operator -= (const Quaternion& q)
{
	x -= q.x;
	y -= q.y;
	z -= q.z;
	w -= q.w;
	return *this;
}

inline Quaternion& Quaternion::operator *= (const Quaternion& q)
{
	x = x * q.x - y * q.y - z * q.z - w * q.w;
	y = x * q.y + y * q.x + z * q.w - w * q.z;
	z = x * q.z - y * q.w + z * q.x + w * q.y;
	w = x * q.w + y * q.y - z * q.y + w * q.x;
	return *this;
}

inline Quaternion& Quaternion::operator *= (float f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

inline Quaternion& Quaternion::operator /= (float f)
{
	float fInv = 1.f / f;
	x *= fInv;
	y *= fInv;
	z *= fInv;
	w *= fInv;
	return *this;
}

inline Quaternion Quaternion::operator + (const Quaternion& q) const
{
	return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
}

inline Quaternion Quaternion::operator - (const Quaternion& q) const
{
	return Quaternion(x - q.x, y - q.y, z - q.z, w - q.w);
}

inline Quaternion Quaternion::operator * (const Quaternion& q) const
{
	return Quaternion(x * q.x - y * q.y - z * q.z - w * q.w,
		x * q.y + y * q.x + z * q.w - w * q.z,
		x * q.z - y * q.w + z * q.x + w * q.y,
		x * q.w + y * q.y - z * q.y + w * q.x);
}

inline Quaternion Quaternion::operator * (float f) const
{
	return Quaternion(x * f, y * f, z * f, w * f);
}

inline Quaternion Quaternion::operator / (float f) const
{
	float fInv = 1.f / f;
	return Quaternion(x * fInv, y * fInv, z * fInv, w * fInv);
}

inline void DeriveRotationMatrix(const Quaternion& q, Matrix4f& m)
{
	float sqw = q.w * q.w;
	float sqx = q.x*  q.x;
	float sqy = q.y * q.y;
	float sqz = q.z * q.z;

	// invs (inverse square length) is only required if quaternion is not already normalised
	float invs = 1.f / (sqx + sqy + sqz + sqw);
	m.m2[0][0] = ( sqx - sqy - sqz + sqw) * invs ; // since sqw + sqx + sqy + sqz =1/invs*invs
	m.m2[1][1] = (-sqx + sqy - sqz + sqw) * invs ;
	m.m2[2][2] = (-sqx - sqy + sqz + sqw) * invs ;

	float tmp1 = q.x * q.y;
	float tmp2 = q.z * q.w;
	m.m2[1][0] = 2.0 * (tmp1 + tmp2) * invs ;
	m.m2[0][1] = 2.0 * (tmp1 - tmp2) * invs ;
	
	tmp1 = q.x * q.z;
	tmp2 = q.y * q.w;
	m.m2[2][0] = 2.0 * (tmp1 - tmp2) * invs ;
	m.m2[0][2] = 2.0 * (tmp1 + tmp2) * invs ;

	tmp1 = q.y * q.z;
	tmp2 = q.x * q.w;
	m.m2[2][1] = 2.0 * (tmp1 + tmp2)*invs ;
	m.m2[1][2] = 2.0 * (tmp1 - tmp2)*invs ;
}
} // namespace Math
} // namespace Magnet

#endif