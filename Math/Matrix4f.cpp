#include "Matrix4f.h"
#include "Vector4f.h"

namespace Magnet
{
namespace Math
{
//------------------------------------------------------------------
Matrix4f::Matrix4f() 
{
	// make default matrix be identity matrix
	for (int i = 0; i < 16; ++i)
		m1[i] = 0.0f;
	m2[0][0] = 1.0f;
	m2[1][1] = 1.0f;
	m2[2][2] = 1.0f;
	m2[3][3] = 1.0f;
}

//------------------------------------------------------------------
Matrix4f::Matrix4f(const float * m)
{
	for (int i = 0; i < 16; ++i)
		m1[i] = m[i];
}

//------------------------------------------------------------------
Matrix4f::Matrix4f(const Matrix4f& mat)
{
	for (int i = 0; i < 16; ++i)
		m1[i] = mat.m1[i];
}

//------------------------------------------------------------------
Matrix4f::Matrix4f(const float c)
{
	for (int i = 0; i < 16; ++i)
		m1[i] = c;
}

//------------------------------------------------------------------
Matrix4f::Matrix4f( float f00, float f01, float f02, float f03,
		  float f10, float f11, float f12, float f13,
		  float f20, float f21, float f22, float f23,
		  float f30, float f31, float f32, float f33)
{
	// array can't use initialization list
	m2[0][0] = f00; m2[0][1] = f01; m2[0][2] = f02; m2[0][3] = f03;
	m2[1][0] = f10; m2[1][1] = f11; m2[1][2] = f12; m2[1][3] = f13;
	m2[2][0] = f20; m2[2][1] = f21; m2[2][2] = f22; m2[2][3] = f23;
	m2[3][0] = f30; m2[3][1] = f31; m2[3][2] = f32; m2[3][3] = f33;
}

//------------------------------------------------------------------
Matrix4f& Matrix4f::operator=(const Matrix4f& mat)
{
	for (int i = 0; i < 16; ++i)
		m1[i] = mat.m1[i];
	return *this;
}

//------------------------------------------------------------------
Matrix4f Matrix4f::operator+(const Matrix4f& mat) const
{
	return Matrix4f( 
		m2[0][0] + mat.m2[0][0],
		m2[0][1] + mat.m2[0][1],
		m2[0][2] + mat.m2[0][2],
		m2[0][3] + mat.m2[0][3],
		m2[1][0] + mat.m2[1][0],
		m2[1][1] + mat.m2[1][1],
		m2[1][2] + mat.m2[1][2],
		m2[1][3] + mat.m2[1][3],
		m2[2][0] + mat.m2[2][0],
		m2[2][1] + mat.m2[2][1],
		m2[2][2] + mat.m2[2][2],
		m2[2][3] + mat.m2[2][3],
		m2[3][0] + mat.m2[3][0],
		m2[3][1] + mat.m2[3][1],
		m2[3][2] + mat.m2[3][2],
		m2[3][3] + mat.m2[3][3]
	);
}

//------------------------------------------------------------------
Matrix4f Matrix4f::operator-(const Matrix4f& mat) const
{
	return Matrix4f( 
		m2[0][0] - mat.m2[0][0],
		m2[0][1] - mat.m2[0][1],
		m2[0][2] - mat.m2[0][2],
		m2[0][3] - mat.m2[0][3],
		m2[1][0] - mat.m2[1][0],
		m2[1][1] - mat.m2[1][1],
		m2[1][2] - mat.m2[1][2],
		m2[1][3] - mat.m2[1][3],
		m2[2][0] - mat.m2[2][0],
		m2[2][1] - mat.m2[2][1],
		m2[2][2] - mat.m2[2][2],
		m2[2][3] - mat.m2[2][3],
		m2[3][0] - mat.m2[3][0],
		m2[3][1] - mat.m2[3][1],
		m2[3][2] - mat.m2[3][2],
		m2[3][3] - mat.m2[3][3]
	);
}

//------------------------------------------------------------------
Matrix4f Matrix4f::operator*(const Matrix4f& mat) const
{
	Matrix4f temp_matrix(0.0f);
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			for (int k = 0; k < 4; ++k)
				temp_matrix.m2[i][j] += m2[i][k] * mat.m2[k][j];
		}
	}
	return temp_matrix;
}

//------------------------------------------------------------------
Matrix4f Matrix4f::GetTranspos() const
{
	Matrix4f temp_matrix(0.f);
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			temp_matrix.m2[i][j] = m2[j][i];
		}
	}
	return temp_matrix;
}
} // namespace Math
} // namespace Magnet