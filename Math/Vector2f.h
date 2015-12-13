#ifndef VECTOR2F_H
#define VECTOR2F_H

#include "Macros.h"

namespace Magnet
{
namespace Math
{

class Vector2f
{
public:
	Vector2f(); // default constructor
	Vector2f(float fx, float fy); // constructor
	Vector2f(const Vector2f& v2); // copy constructor
	Vector2f(const float f);
	
	Vector2f& operator = (const Vector2f& v2); // assignment operator

	// operator overloading
	const Vector2f& operator+() const;
	Vector2f operator-() const;
	Vector2f operator+(const Vector2f& v2) const;
	Vector2f& operator+=(const Vector2f& v2);
	Vector2f operator-(const Vector2f& v2) const;
	Vector2f& operator-=(const Vector2f &v2);
	Vector2f operator/(const float f) const;
	Vector2f& operator/=(const float f);
	Vector2f operator*(const float f) const;
	Vector2f& operator*=(const float f);

	float Length() const;
	float SquareLength() const;
	void Normalize();

	// the result not use static function is that static can't be inlined outside the class scope
	//inline static Vector2f Normalize(const Vector2f& v2);

public:
	float x, y;
	
};

//------------------------------------------------------------------
inline Vector2f::Vector2f() : x(0.0f), y(0.0f) {}

//------------------------------------------------------------------
inline Vector2f::Vector2f(float fx, float fy) : x(fx), y(fy) {}

//------------------------------------------------------------------
inline Vector2f::Vector2f(const Vector2f& v2) : x(v2.x), y(v2.y) {}

//------------------------------------------------------------------
inline Vector2f::Vector2f(const float f) : x(f), y(f) {}

//------------------------------------------------------------------
inline Vector2f& Vector2f::operator = (const Vector2f& v2) 
{
	x = v2.x;
	y = v2.y;
	return *this;
}

//------------------------------------------------------------------
inline const Vector2f& Vector2f::operator+() const
{
	return *this;
}

//------------------------------------------------------------------
inline Vector2f Vector2f::operator-() const
{
	return Vector2f(-x, -y);
}

//------------------------------------------------------------------
inline Vector2f Vector2f::operator+(const Vector2f& v2) const
{
	return Vector2f(x + v2.x, y + v2.y);
}

//------------------------------------------------------------------
/*binary operator*/
//	Vector2f operator+(const Vector2f& v2A, const Vector2f& v2B) const {
//		return Vector2f(v2A.x + v2B.x, v2A.y + v2B.y);
//	}

//------------------------------------------------------------------
inline Vector2f& Vector2f::operator+=(const Vector2f& v2)
{
	x += v2.x;
	y += v2.y;
	return *this;
}

//------------------------------------------------------------------
inline Vector2f Vector2f::operator-(const Vector2f& v2) const
{
	return Vector2f(x - v2.x, y - v2.y);
}

//------------------------------------------------------------------
inline Vector2f& Vector2f::operator-=(const Vector2f &v2)
{
	x -= v2.x;
	y -= v2.y;
	return *this;
}

//------------------------------------------------------------------
inline Vector2f Vector2f::operator*(const float f) const
{
	return Vector2f(x * f, y * f);
}

//------------------------------------------------------------------
inline Vector2f& Vector2f::operator*=(const float f)
{
	x *= f;
	y *= f;
	return *this;
}

//------------------------------------------------------------------
// unary operator, can't do f*v2, but can do v2*f
//	friend Vector2f operator*(const Vector2f& v2, const float f);
//	friend Vector2f operator*(const float f, const Vector2f& v2) const;
//	Vector2f operator*(const Vector2f& v2, const float f) const {				// binary operator
//		return Vector2f(x * f, y * f);
//	}
//	Vector2f operator*(const float f, const Vector2f& v2) const {				// binary operator
//		return Vector2f(x * f, y * f);
//	}

//------------------------------------------------------------------
inline Vector2f Vector2f::operator/(const float f) const
{
	return Vector2f(x / f, y / f);
}

//------------------------------------------------------------------
inline Vector2f& Vector2f::operator/=(const float f)
{
	x /= f;
	y /= f;
	return *this;
}

//------------------------------------------------------------------
inline float Vector2f::Length() const
{
	return sqrtf(x * x + y * y);
}

//------------------------------------------------------------------
inline float Vector2f::SquareLength() const
{
	return x * x + y * y;
}

//------------------------------------------------------------------
inline void Vector2f::Normalize()
{
	float r = Length();
	x /= r;
	y /= r;
}

//------------------------------------------------------------------
inline Vector2f Normalize(const Vector2f& v2)
{
	float r = v2.Length();
	return Vector2f(v2.x / r, v2.y / r);
}

//------------------------------------------------------------------
// one way to define binary operator is to use inline
/*
inline Vector2f operator*(const Vector2f& v2, const float f) {		// binary operator
	return Vector2f(v2.x * f, v2.y * f);
}

inline Vector2f operator*(const float f, const Vector2f& v2) {		// binary operator
	return Vector2f(v2.x * f, v2.y * f);
}
*/

} // namespace Math
} // namespace Magnet
#endif