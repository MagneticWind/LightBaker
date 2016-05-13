#include "Math.h"

#include "Matrix4f.h"
#include "Vector3f.h"
#include "Vector4f.h"

namespace Magnet
{
namespace Math
{
//@NOTE: this function compute the tranpose matrix of : http://msdn.microsoft.com/en-us/library/windows/desktop/bb281710(v=vs.85).aspx
//       When we submite the matrix to GPU, we don't need another transpose operation
void MatrixLookAtLH(const Math::Vector3f& v3Position, const Math::Vector3f& v3LookAt, const Math::Vector3f& v3Up, Math::Matrix4f& mView)
{
	Math::Vector3f v3ZAxis = v3LookAt - v3Position;
	v3ZAxis.Normalize();

	Math::Vector3f v3XAxis;
	if(abs(1.f - abs(v3ZAxis.y)) < 0.00001f)
		v3XAxis = Math::Vector3f(-1.f, 0.f, 0.f);
	else
		v3XAxis = Cross3f(v3Up, v3ZAxis);

	v3XAxis.Normalize();
	Math::Vector3f v3YAxis = Cross3f(v3ZAxis, v3XAxis);
	v3YAxis.Normalize();

	mView.m2[0][0] = v3XAxis.x; mView.m2[0][1] = v3XAxis.y; mView.m2[0][2] = v3XAxis.z; mView.m2[0][3] = - Dot3f(v3XAxis, v3Position);
	mView.m2[1][0] = v3YAxis.x; mView.m2[1][1] = v3YAxis.y; mView.m2[1][2] = v3YAxis.z; mView.m2[1][3] = - Dot3f(v3YAxis, v3Position);
	mView.m2[2][0] = v3ZAxis.x; mView.m2[2][1] = v3ZAxis.y; mView.m2[2][2] = v3ZAxis.z; mView.m2[2][3] = - Dot3f(v3ZAxis, v3Position);
	mView.m2[3][0] =		 0; mView.m2[3][1] =		 0; mView.m2[3][2] =		 0; mView.m2[3][3] =						   1;
}


void MatrixLookAtRH(const Math::Vector3f& v3Position, const Math::Vector3f& v3LookAt, const Math::Vector3f& v3Up, Math::Matrix4f& mView)
{
	Math::Vector3f v3ZAxis = v3Position - v3LookAt;
	v3ZAxis.Normalize();
	Math::Vector3f v3XAxis = Cross3f(v3Up, v3ZAxis);
	v3XAxis.Normalize();
	Math::Vector3f v3YAxis = Cross3f(v3ZAxis, v3XAxis);
	v3YAxis.Normalize();

	mView.m2[0][0] = v3XAxis.x; mView.m2[0][1] = v3XAxis.y; mView.m2[0][2] = v3XAxis.z; mView.m2[0][3] = - Dot3f(v3XAxis, v3Position);
	mView.m2[1][0] = v3YAxis.x; mView.m2[1][1] = v3YAxis.y; mView.m2[1][2] = v3YAxis.z; mView.m2[1][3] = - Dot3f(v3YAxis, v3Position);
	mView.m2[2][0] = v3ZAxis.x; mView.m2[2][1] = v3ZAxis.y; mView.m2[2][2] = v3ZAxis.z; mView.m2[2][3] = - Dot3f(v3ZAxis, v3Position);
	mView.m2[3][0] =		 0; mView.m2[3][1] =		 0; mView.m2[3][2] =		 0; mView.m2[3][3] =						   1;
}

//@NOTE: this function is compute transpose of matrix in: https://msdn.microsoft.com/en-us/library/windows/desktop/bb281727(v=vs.85).aspx
//       When we submite the matrix to GPU, we don't need another transpose operation
void MatrixPerspectiveFovLH(float fFovAngle, float fAspectRatio, float fZNear, float fZFar, Math::Matrix4f& mProjection)
{
	float fYScale = 1.0 / tan(fFovAngle * 0.5);
	float fXScale = fYScale / fAspectRatio;
	mProjection.m2[0][0] = fXScale; mProjection.m2[0][1] =       0; mProjection.m2[0][2] = 0; mProjection.m2[0][3] = 0;
	mProjection.m2[1][0] =       0; mProjection.m2[1][1] = fYScale; mProjection.m2[1][2] = 0; mProjection.m2[1][3] = 0;
	mProjection.m2[2][0] =       0; mProjection.m2[2][1] =       0; mProjection.m2[2][2] = fZFar / (fZFar-fZNear); mProjection.m2[2][3] = -fZFar*fZNear/(fZFar-fZNear);
	mProjection.m2[3][0] =		 0; mProjection.m2[3][1] =		 0; mProjection.m2[3][2] = 1; mProjection.m2[3][3] = 0;
}

void MatrixOrthographicLH(float fWidth, float fHeight, float fZNear, float fZFar, Math::Matrix4f& mProjection)
{
	mProjection.m2[0][0] = 2.0/fWidth; mProjection.m2[0][1] =       0; mProjection.m2[0][2] = 0; mProjection.m2[0][3] = 0;
	mProjection.m2[1][0] =       0; mProjection.m2[1][1] = 2.0/fHeight; mProjection.m2[1][2] = 0; mProjection.m2[1][3] = 0;
	mProjection.m2[2][0] =       0; mProjection.m2[2][1] =       0; mProjection.m2[2][2] = 1.0 / (fZFar-fZNear); mProjection.m2[2][3] = -fZNear/(fZFar-fZNear);
	mProjection.m2[3][0] =		 0; mProjection.m2[3][1] =		 0; mProjection.m2[3][2] = 0; mProjection.m2[3][3] = 1;
}

void MatrixOrthographicOffCenterLH( float fMinX, float fMaxX, float fMinY, float fMaxY, float fMinZ, float fMaxZ, Math::Matrix4f& mProjection)
{
	mProjection.m2[0][0] = 2.0/(fMaxX-fMinX); mProjection.m2[0][1] =       0; mProjection.m2[0][2] = 0; mProjection.m2[0][3] = (fMinX+fMaxX)/(fMinX-fMaxX);
	mProjection.m2[1][0] =       0; mProjection.m2[1][1] = 2.0/(fMaxY-fMinY); mProjection.m2[1][2] = 0; mProjection.m2[1][3] = (fMinY+fMaxY)/(fMinY-fMaxY);
	mProjection.m2[2][0] =       0; mProjection.m2[2][1] =       0; mProjection.m2[2][2] = 1.0/(fMaxZ-fMinZ); mProjection.m2[2][3] = fMinZ/(fMinZ-fMaxZ);
	mProjection.m2[3][0] =		 0; mProjection.m2[3][1] =		 0; mProjection.m2[3][2] = 0; mProjection.m2[3][3] = 1;
}

// Rotate by Y axis, X axis, Z axis
// Cos(z).Cos(y) + Sin(z).Sin(x).Sin(y)				Sin(z).Cos(x)			Cos(z).-Sin(y) + Sin(z).Sin(x).Cos(y)
// -Sin(z).Cos(y) + Cos(z).Sin(x).Sin(y)			Cos(z).Cos(x)			Sin(z).Sin(y) + Cos(z).Sin(x).Cos(y)
// Cos(x).Sin(y)									-Sin(x)					Cos(x).Cos(y)
void MatrixRotationYawPitchRoll(float fYawAngle, float fPitchAngle, float fRollAngle, Math::Matrix4f& mRotation)
{
	float sx = sin(fPitchAngle);
	float sy = sin(fYawAngle);
	float sz = sin(fRollAngle);
	float cx = cos(fPitchAngle);
	float cy = cos(fYawAngle);
	float cz = cos(fRollAngle);

	mRotation.m2[0][0] = cz * cy + sx * sy * sz;
	mRotation.m2[0][1] = sz * cx;
	mRotation.m2[0][2] = -cz * sy + sz * sx * cy;
	mRotation.m2[0][3] = 0.0f;

	mRotation.m2[1][0] = -sz * cy + cz * sx * sy;
	mRotation.m2[1][1] = cx * cz;
	mRotation.m2[1][2] = sz * sy + cz * sx * cy;
	mRotation.m2[1][3] = 0.0f;

	mRotation.m2[2][0] = cx * sy;
	mRotation.m2[2][1] = -sx;
	mRotation.m2[2][2] = cx * cy;
	mRotation.m2[2][3] = 0.0f;

	mRotation.m2[3][0] = 0.0f;
	mRotation.m2[3][1] = 0.0f;
	mRotation.m2[3][2] = 0.0f;
	mRotation.m2[3][3] = 1.0f;
}

// rotate by x, y, z axies
void GetTranslationMatrix(const Math::Vector3f& v3Translation, Matrix4f& mTranslation)
{
	mTranslation.m2[0][0] = 1.f; mTranslation.m2[0][1] = 0.f; mTranslation.m2[0][2] = 0.f; mTranslation.m2[0][3] = v3Translation.x;
	mTranslation.m2[1][0] = 0.f; mTranslation.m2[1][1] = 1.f; mTranslation.m2[1][2] = 0.f; mTranslation.m2[1][3] = v3Translation.y;
	mTranslation.m2[2][0] = 0.f; mTranslation.m2[2][1] = 0.f; mTranslation.m2[2][2] = 1.f; mTranslation.m2[2][3] = v3Translation.z;
	mTranslation.m2[3][0] = 0.f; mTranslation.m2[3][1] = 0.f; mTranslation.m2[3][2] = 0.f; mTranslation.m2[3][3] = 1.f;
}

void GetRotationMatrix(const Math::Vector3f& v3Rotation, Matrix4f& mRotation)
{
	MatrixRotationYawPitchRoll(v3Rotation.y, v3Rotation.x, v3Rotation.z, mRotation);
}

void GetScaleMatrix(const Math::Vector3f& v3Scale, Matrix4f& mScale)
{
	mScale.m2[0][0] = v3Scale.x; mScale.m2[0][1] = 0.f;       mScale.m2[0][2] = 0.f;       mScale.m2[0][3] = 0.f;
	mScale.m2[1][0] = 0.f;       mScale.m2[1][1] = v3Scale.y; mScale.m2[1][2] = 0.f;       mScale.m2[1][3] = 0.f;
	mScale.m2[2][0] = 0.f;       mScale.m2[2][1] = 0.f;       mScale.m2[2][2] = v3Scale.z; mScale.m2[2][3] = 0.f;
	mScale.m2[3][0] = 0.f;       mScale.m2[3][1] = 0.f;       mScale.m2[3][2] = 0.f;       mScale.m2[3][3] = 1.f;
}

} // namespace Math
} // namespace Magnet