#ifndef MATRIX4F_H
#define MATRIX4F_H

#include "Vector3f.h"
#include "Vector4f.h"

namespace Magnet
{
namespace Math
{
class Vector4f;

class Matrix4f
{
public:
	Matrix4f(); // default constructor
	Matrix4f(const float * m);
	Matrix4f(const Matrix4f& mat); // copy constructor
	Matrix4f(const float c);
	Matrix4f( float f00, float f01, float f02, float f03,
			  float f10, float f11, float f12, float f13,
			  float f20, float f21, float f22, float f23,
			  float f30, float f31, float f32, float f33);
	
	// assignment operator
	Matrix4f& operator=(const Matrix4f& mat);
	Matrix4f operator+(const Matrix4f& mat) const;
	Matrix4f operator-(const Matrix4f& mat) const;
	Matrix4f operator*(const Matrix4f& mat) const;
	Matrix4f GetTranspos() const;

public:
	union
	{
		float m2[4][4];        // 2d array
		float m1[16];          // 1d array
	};
};

inline Vector4f operator*(const Matrix4f& mat4, const Vector4f& vec4)
{
	float x = mat4.m1[0]*vec4.x + mat4.m1[1]*vec4.y + mat4.m1[2]*vec4.z + mat4.m1[3]*vec4.w;
	float y = mat4.m1[4]*vec4.x + mat4.m1[5]*vec4.y + mat4.m1[6]*vec4.z + mat4.m1[7]*vec4.w;
	float z = mat4.m1[8]*vec4.x + mat4.m1[9]*vec4.y + mat4.m1[10]*vec4.z + mat4.m1[11]*vec4.w;
	float w = mat4.m1[12]*vec4.x + mat4.m1[13]*vec4.y + mat4.m1[14]*vec4.z + mat4.m1[15]*vec4.w;
	return Vector4f(x,y,z,w);
}

inline Vector3f operator*(const Matrix4f& mat4, const Vector3f& vec3)
{
	Vector4f temp(vec3.x, vec3.y, vec3.z, 1.f);
	Vector4f result = mat4 * temp;
	return Vector3f(result.x / result.w, result.y / result.w, result.z / result.w);
}

} // namespace Math
} // namespace Magnet

#endif