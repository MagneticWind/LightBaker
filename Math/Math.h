#ifndef MATH_H
#define MATH_H

namespace Magnet
{
namespace Math
{

class Vector3f;
class Vector4f;
class Matrix4f;

void MatrixLookAtLH(const Math::Vector3f& v3Position, const Math::Vector3f& v3LookAt, const Math::Vector3f& v3Up, Math::Matrix4f& mView);
void MatrixLookAtRH(const Math::Vector3f& v3Position, const Math::Vector3f& v3LookAt, const Math::Vector3f& v3Up, Math::Matrix4f& mView);
void MatrixPerspectiveFovLH(float fFovAngle, float fAspectRatio, float fZNear, float fZFar, Math::Matrix4f& mProjection);
void MatrixOrthographicLH(float fWidth, float fHeight, float fZNear, float fZFar, Math::Matrix4f& mProjection);
void MatrixOrthographicOffCenterLH( float fMinX, float fMaxX, float fMinY, float fMaxY, float fMinZ, float fMaxZ, Math::Matrix4f& mProjection);
void MatrixRotationYawPitchRoll(float fYawAngle, float fPitchAngle, float fRollAngle, Math::Matrix4f& mRotation);

void GetTranslationMatrix(const Math::Vector3f& v3Translation, Matrix4f& mTranslation);
void GetRotationMatrix(const Math::Vector3f& v3Rotation, Matrix4f& mRotation);
void GetScaleMatrix(const Math::Vector3f& v3Scale, Matrix4f& mScale);

} // namespace Math
} // namespace Magnet

#endif