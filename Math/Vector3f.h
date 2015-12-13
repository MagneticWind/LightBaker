#ifndef VECTOR3F_H
#define VECTOR3F_H

#include "Macros.h"

namespace Magnet
{
namespace Math
{
class Vector3f
{
public:
	Vector3f(); // default constructor
	Vector3f(float fx, float fy, float fz); // constructor
	Vector3f(const Vector3f& v3); // copy constructor
	Vector3f(const float f);
	
	Vector3f& operator = (const Vector3f& v3); // assignment operator

	// operator overloading
	const Vector3f& operator+() const;
	Vector3f operator-() const;
	Vector3f operator+(const Vector3f& v3) const;
	Vector3f& operator+=(const Vector3f& v3);
	Vector3f operator-(const Vector3f& v3) const;
	Vector3f& operator-=(const Vector3f &v3);
	Vector3f operator/(const float f) const;
	Vector3f& operator/=(const float f);
	Vector3f operator*(const float f) const;
	Vector3f& operator*=(const float f);

	float Length() const;
	float SquareLength() const;
	void Normalize();

public:
	float x, y, z;
	
};

//------------------------------------------------------------------
inline Vector3f::Vector3f() : x(0.0f), y(0.0f), z(0.0f) {}

//------------------------------------------------------------------
inline Vector3f::Vector3f(float fx, float fy, float fz) : x(fx),y(fy),z(fz) {}

//------------------------------------------------------------------
inline Vector3f::Vector3f(const Vector3f& v3) : x(v3.x), y(v3.y), z(v3.z) {}

//------------------------------------------------------------------
inline Vector3f::Vector3f(const float f) : x(f), y(f), z(f) {}

//------------------------------------------------------------------
inline Vector3f& Vector3f::operator = (const Vector3f& v3) 
{
	x = v3.x;
	y = v3.y;
	z = v3.z;
	return *this;
}

//------------------------------------------------------------------
inline const Vector3f& Vector3f::operator+() const
{
	return *this;
}

//------------------------------------------------------------------
inline Vector3f Vector3f::operator-() const
{
	return Vector3f(-x, -y, -z);
}

//------------------------------------------------------------------
inline Vector3f Vector3f::operator+(const Vector3f& v3) const
{
	return Vector3f(x + v3.x, y + v3.y, z + v3.z);
}

//------------------------------------------------------------------
/*binary operator*/
//	Vector3f operator+(const Vector3f& v3A, const Vector3f& v3B) const {
//		return Vector3f(v3A.x + v3B.x, v3A.y + v3B.y, v3A.z + v3B.z);
//	}

//------------------------------------------------------------------
inline Vector3f& Vector3f::operator+=(const Vector3f& v3)
{
	x += v3.x;
	y += v3.y;
	z += v3.z;
	return *this;
}

//------------------------------------------------------------------
inline Vector3f Vector3f::operator-(const Vector3f& v3) const
{
	return Vector3f(x - v3.x, y - v3.y, z - v3.z);
}

//------------------------------------------------------------------
inline Vector3f& Vector3f::operator-=(const Vector3f &v3)
{
	x -= v3.x;
	y -= v3.y;
	z -= v3.z;
	return *this;
}

//------------------------------------------------------------------
inline Vector3f Vector3f::operator*(const float f) const
{
	return Vector3f(x * f, y * f, z * f);
}

//------------------------------------------------------------------
inline Vector3f& Vector3f::operator*=(const float f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

//------------------------------------------------------------------
// unary operator, can't do f*v3, but can do v3*f
//	friend Vector3f operator*(const Vector3f& v3, const float f);
//	friend Vector3f operator*(const float f, const Vector3f& v3) const;
//	Vector3f operator*(const Vector3f& v3, const float f) const {				// binary operator
//		return Vector3f(x * f, y * f, z * f);
//	}
//	Vector3f operator*(const float f, const Vector3f& v3) const {				// binary operator
//		return Vector3f(x * f, y * f, z * f);
//	}

//------------------------------------------------------------------
inline Vector3f Vector3f::operator/(const float f) const
{
	return Vector3f(x / f, y / f, z / f);
}

//------------------------------------------------------------------
inline Vector3f& Vector3f::operator/=(const float f)
{
	x /= f;
	y /= f;
	z /= f;
	return *this;

}

//------------------------------------------------------------------
inline float Vector3f::Length() const
{
	return sqrtf(x * x + y * y + z * z);
}

//------------------------------------------------------------------
inline float Vector3f::SquareLength() const
{
	return x * x + y * y + z * z;
}

//------------------------------------------------------------------
inline void Vector3f::Normalize()
{
	float r = Length();
	x /= r;
	y /= r;
	z /= r;
}

//------------------------------------------------------------------
inline Vector3f Normalize(const Vector3f& v3)
{
	float r = v3.Length();
	return Vector3f(v3.x / r, v3.y / r, v3.z / r);
}

//------------------------------------------------------------------
// one way to define binary operator is to use inline
/*
inline Vector3f operator*(const Vector3f& v3, const float f) {		// binary operator
	return Vector3f(v3.x * f, v3.y * f, v3.z * f);
}

inline Vector3f operator*(const float f, const Vector3f& v3) {		// binary operator
	return Vector3f(v3.x * f, v3.y * f, v3.z * f);
}
*/

//------------------------------------------------------------------
inline float Dot3f(const Vector3f& v3A, const Vector3f& v3B)
{
	return v3A.x * v3B.x + v3A.y * v3B.y + v3A.z * v3B.z;
}

//------------------------------------------------------------------
// cross product respects both Left handed system and right handed system
inline Vector3f Cross3f(const Vector3f& v3A, const Vector3f& v3B)
{
	return Vector3f(v3A.y * v3B.z - v3A.z * v3B.y,
				    v3A.z * v3B.x - v3A.x * v3B.z,
				    v3A.x * v3B.y - v3A.y * v3B.x);
}

// the result not use static function is that static can't be inlined outside the class scope
//inline Vector3f Normalize(const Vector3f& v3);
//inline float Dot3f(const Vector3f& v3A, const Vector3f& v3B);
//inline Vector3f Cross3f(const Vector3f& v3A, const Vector3f& v3B);

} // namespace Math
} // namespace Magnet
#endif