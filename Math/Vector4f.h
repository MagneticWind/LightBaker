#ifndef VECTOR4F_H
#define VECTOR4F_H

#include "Macros.h"

namespace Magnet
{
namespace Math
{
class Vector4f
{
public:
	Vector4f(); // default constructor
	Vector4f(float fx, float fy, float fz, float fw); // constructor
	Vector4f(const Vector4f& v4); // copy constructor
	Vector4f(const float f);

	Vector4f& operator = (const Vector4f& v4); // assignment operator

	// operator overloading
	const Vector4f& operator+() const;
	Vector4f operator-() const;

	Vector4f& operator+=(const Vector4f& v4);
	Vector4f& operator-=(const Vector4f &v4);
	Vector4f& operator*=(const float f);
	Vector4f& operator/=(const float f);

	Vector4f operator+(const Vector4f& v4) const;
	Vector4f operator-(const Vector4f& v4) const;
	Vector4f operator*(const float f) const;
	Vector4f operator/(const float f) const;

	float Length() const;
	float SquareLength() const;
	void Normalize();

	// the result not use static function is that static can't be inlined outside the class scope
	//inline static Vector4f Normalize(const Vector4f& v4);

public:
	float x, y, z, w;
};

//------------------------------------------------------------------
inline Vector4f::Vector4f() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

//------------------------------------------------------------------
inline Vector4f::Vector4f(float fx, float fy, float fz, float fw) : x(fx),y(fy),z(fz), w(fw) {}

//------------------------------------------------------------------
inline Vector4f::Vector4f(const Vector4f& v4) : x(v4.x), y(v4.y), z(v4.z), w(v4.w) {}

//------------------------------------------------------------------
inline Vector4f::Vector4f(const float f) : x(f), y(f), z(f), w(f) {}

//------------------------------------------------------------------
inline Vector4f& Vector4f::operator = (const Vector4f& v4) 
{
	x = v4.x;
	y = v4.y;
	z = v4.z;
	w = v4.w;
	return *this;
}

//------------------------------------------------------------------
inline const Vector4f& Vector4f::operator+() const
{
	return *this;
}

//------------------------------------------------------------------
inline Vector4f Vector4f::operator-() const
{
	return Vector4f(-x, -y, -z, -w);
}

//------------------------------------------------------------------
inline Vector4f Vector4f::operator+(const Vector4f& v4) const
{
	return Vector4f(x + v4.x, y + v4.y, z + v4.z, w + v4.w);
}

//------------------------------------------------------------------
/*binary operator*/
//	Vector3f operator+(const Vector3f& v3A, const Vector3f& v3B) const {
//		return Vector3f(v3A.x + v3B.x, v3A.y + v3B.y, v3A.z + v3B.z);
//	}

//------------------------------------------------------------------
inline Vector4f& Vector4f::operator+=(const Vector4f& v4)
{
	x += v4.x;
	y += v4.y;
	z += v4.z;
	w += v4.w;
	return *this;
}

//------------------------------------------------------------------
inline Vector4f Vector4f::operator-(const Vector4f& v4) const
{
	return Vector4f(x - v4.x, y - v4.y, z - v4.z, w - v4.w);
}

//------------------------------------------------------------------
inline Vector4f& Vector4f::operator-=(const Vector4f &v4)
{
	x -= v4.x;
	y -= v4.y;
	z -= v4.z;
	w -= v4.w;
	return *this;
}

//------------------------------------------------------------------
inline Vector4f Vector4f::operator*(const float f) const
{
	return Vector4f(x * f, y * f, z * f, w * f);
}

//------------------------------------------------------------------
inline Vector4f& Vector4f::operator*=(const float f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

//------------------------------------------------------------------
// unary operator, can't do f*v4, but can do v4*f
//	friend Vector3f operator*(const Vector3f& v3, const float f);
//	friend Vector3f operator*(const float f, const Vector3f& v3) const;
//	Vector3f operator*(const Vector3f& v3, const float f) const {				// binary operator
//		return Vector3f(x * f, y * f, z * f);
//	}
//	Vector3f operator*(const float f, const Vector3f& v3) const {				// binary operator
//		return Vector3f(x * f, y * f, z * f);
//	}

//------------------------------------------------------------------
inline Vector4f Vector4f::operator/(const float f) const
{
	return Vector4f(x / f, y / f, z / f, w / f);
}

//------------------------------------------------------------------
inline Vector4f& Vector4f::operator/=(const float f)
{
	x /= f;
	y /= f;
	z /= f;
	w /= f;
	return *this;
}

//------------------------------------------------------------------
inline float Vector4f::Length() const
{
	return sqrtf(x * x + y * y + z * z + w * w);
}

//------------------------------------------------------------------
inline float Vector4f::SquareLength() const
{
	return x * x + y * y + z * z + w * w;
}

//------------------------------------------------------------------
inline void Vector4f::Normalize()
{
	float r = Length();
	x /= r;
	y /= r;
	z /= r;
	w /= r;
}

//------------------------------------------------------------------
inline Vector4f Normalize(const Vector4f& v4)
{
	float r = v4.Length();
	return Vector4f(v4.x / r, v4.y / r, v4.z / r, v4.w / r);
}

//------------------------------------------------------------------
// one way to define binary operator is to use inline
/*
inline Vector4f operator*(const Vector4f& v4, const float f) {		// binary operator
	return Vector4f(v4.x * f, v4.y * f, v4.z * f, v4.w * f);
}

inline Vector4f operator*(const float f, const Vector4f& v4) {		// binary operator
	return Vector4f(v4.x * f, v4.y * f, v4.z * f, v4.w * f);
}
*/

} // namespace Math
} // namespace Magnet
#endif