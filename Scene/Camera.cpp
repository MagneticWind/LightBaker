#include "Math\Macros.h"
#include "Camera.h"
#include "Math\Plane.h"
#include "Math\Frustum.h"
#include "Math\Vector4f.h"
#include "Math\Math.h"
#include "Math\Macros.h"
#include "Math\Matrix4f.h"

namespace Magnet
{
namespace Scene
{
//------------------------------------------------------------------
Camera::Camera(const char* pName)
{
	m_bIsPerspective = true;

	m_fForward = 0.f;
	m_fLeft = 0.f;
	m_iRotateX = 0;
	m_iRotateY = 0;

	//default camera parameters
	m_v3Up = Math::Vector3f(0.0, 1.f, 0.0);
	m_fAspectRatio = 8.f / 6.f;
	m_fFOV = 60.f;
	m_fZNear = 0.1f;
	m_fZFar = 10000.f;

	m_fVelocity = 0.1f;
	m_fRotationScaler = 0.01f;

	m_fForward = 0.f;
	m_fLeft = 0.f;

	m_bActive = true;
	m_bDrawable = false;
	m_bVisible = false;

	m_bRotate = false;
	m_bTranslate = false;

	m_bIsInitialized = false;

}

//------------------------------------------------------------------
Camera::~Camera()
{

}

//------------------------------------------------------------------
void Camera::UpdateViewFrustum()
{
	Math::Vector3f v3LookDirection = m_v3Lookat - m_v3Position;
	v3LookDirection.Normalize();

	Math::Vector3f v3Left = Math::Cross3f(m_v3Up, v3LookDirection);
	v3Left.Normalize();

	Math::Vector3f v3YDirection = Math::Cross3f(v3LookDirection, v3Left);
	v3YDirection.Normalize();

	Math::Vector3f v3NearCenter = m_v3Position + v3LookDirection * (m_fZNear);
	Math::Vector3f v3FarCenter = m_v3Position + v3LookDirection * (m_fZFar);

	float fNearHeight = 2.f * tanf(m_fFOV * M_PI / 180.f * 0.5f) * m_fZNear;
	float fFarHeight = 2.f * tanf(m_fFOV * M_PI / 180.f* 0.5f) * m_fZFar;
	float fNearWidth = fNearHeight * m_fAspectRatio;
	float fFarWidth = fFarHeight * m_fAspectRatio;

	// @NOTE: more efficient method exist
	Math::Vector3f v3FarTopLeft = v3FarCenter + v3YDirection * fFarHeight * 0.5f + v3Left * fFarWidth * 0.5f;
	Math::Vector3f v3FarTopRight = v3FarCenter + v3YDirection * fFarHeight * 0.5f - v3Left * fFarWidth * 0.5f;
	Math::Vector3f v3FarBottomLeft = v3FarCenter - v3YDirection * fFarHeight * 0.5f + v3Left * fFarWidth * 0.5f;
	Math::Vector3f v3FarBottomRight = v3FarCenter - v3YDirection * fFarHeight * 0.5f - v3Left * fFarWidth * 0.5f;

	Math::Vector3f v3NearTopLeft = v3NearCenter + v3YDirection * fNearHeight * 0.5f + v3Left * fNearWidth * 0.5f;
	Math::Vector3f v3NearTopRight = v3NearCenter + v3YDirection * fNearHeight * 0.5f - v3Left * fNearWidth * 0.5f;
	Math::Vector3f v3NearBottomLeft = v3NearCenter - v3YDirection * fNearHeight * 0.5f + v3Left * fNearWidth * 0.5f;
	Math::Vector3f v3NearBottomRight = v3NearCenter - v3YDirection * fNearHeight * 0.5f - v3Left * fNearWidth * 0.5f;

	m_frustum.m_CornerPoints[0] = v3NearTopLeft;
	m_frustum.m_CornerPoints[1] = v3NearBottomLeft;
	m_frustum.m_CornerPoints[2] = v3NearBottomRight;
	m_frustum.m_CornerPoints[3] = v3NearTopRight;
	m_frustum.m_CornerPoints[4] = v3FarTopLeft;
	m_frustum.m_CornerPoints[5] = v3FarBottomLeft;
	m_frustum.m_CornerPoints[6] = v3FarBottomRight;
	m_frustum.m_CornerPoints[7] = v3FarTopRight;

	//@NOTE: check Frustum.h for order or planes, can't use enum in Frustum, don't know why
	/*m_frustum.m_Planes[0].SetBy3Points(v3FarTopLeft, v3NearTopLeft, v3NearTopRight);
	m_frustum.m_Planes[1].SetBy3Points(v3NearBottomLeft, v3FarBottomLeft, v3FarBottomRight);
	m_frustum.m_Planes[2].SetBy3Points(v3NearBottomLeft, v3NearTopLeft, v3FarTopLeft);
	m_frustum.m_Planes[3].SetBy3Points(v3NearTopRight, v3NearBottomRight, v3FarBottomRight);
	m_frustum.m_Planes[4].SetBy3Points(v3NearTopLeft, v3NearBottomLeft, v3NearBottomRight);
	m_frustum.m_Planes[5].SetBy3Points(v3FarTopLeft, v3FarTopRight, v3FarBottomRight);*/

	m_frustum.m_Planes[0].SetBy3Points(v3FarTopLeft, v3NearTopRight, v3NearTopLeft);
	m_frustum.m_Planes[1].SetBy3Points(v3NearBottomLeft, v3FarBottomRight, v3FarBottomLeft);
	m_frustum.m_Planes[2].SetBy3Points(v3NearBottomLeft, v3FarTopLeft, v3NearTopLeft);
	m_frustum.m_Planes[3].SetBy3Points(v3NearTopRight, v3FarBottomRight, v3NearBottomRight);
	m_frustum.m_Planes[4].SetBy3Points(v3NearTopLeft, v3NearBottomRight, v3NearBottomLeft);
	m_frustum.m_Planes[5].SetBy3Points(v3FarTopLeft, v3FarBottomRight, v3FarTopRight);
}

//------------------------------------------------------------------
void Camera::Update(float fTimeLapse)
{
	//@TODO: design a call back function so whenever mouse or keyboards events,
	//       change camera parameters
	//       also update the aspect ratios if user change the window sizes
	UpdateInput(fTimeLapse);
	UpdateViewFrustum();
	UpdateViewProjectMatrices();
}

//------------------------------------------------------------------
void Camera::UpdateViewProjectMatrices()
{
	// update view matrix
	Math::MatrixLookAtLH(m_v3Position, m_v3Lookat, m_v3Up, m_mView);

	if (!m_bIsInitialized)
	{
		// update yaw and pitch angles
		Math::Vector3f v3ZBasis(m_mView.m2[2][0], m_mView.m2[2][1], m_mView.m2[2][2]);

		m_fRotationYawAngle = -atan2f(v3ZBasis.x, v3ZBasis.z);
		const float fLen = sqrtf(v3ZBasis.z * v3ZBasis.z + v3ZBasis.x * v3ZBasis.x);
		m_fRotationPitchAngle = atan2f(v3ZBasis.y, fLen);

		m_bIsInitialized = true;
	}

	// upate projection matrix
	if (m_bIsPerspective)
	{
		Math::MatrixPerspectiveFovLH(m_fFOV * M_PI / 180.f, m_fAspectRatio, m_fZNear, m_fZFar, m_mProjection);
	}
	else
	{
		//@TODO: add members for orthographic projection
		Math::MatrixOrthographicLH(2.f, 2.f, m_fZNear, m_fZFar, m_mProjection);
	}
}

//------------------------------------------------------------------
void Camera::SetRotationParams(bool bRotate, int iMousePositionX, int iMousePositionY)
{
	m_bRotate = bRotate;
	m_iLastMousePositionX = iMousePositionX;
	m_iLastMousePositionY = iMousePositionY;
}

//------------------------------------------------------------------
void Camera::SetTranslationParams(bool bTranslate, int iMoveForward, int iMoveLeft)
{
	m_bTranslate = bTranslate;
	m_fForward = iMoveForward;
	m_fLeft = iMoveLeft;
}

//------------------------------------------------------------------
void Camera::UpdateInput(float fTimeLapse)
{
	if (m_bRotate)
	{
		// Calc how far it's moved since last frame
		m_callbackGetCurrentMousePosition(m_iCurrentMousePositionX, m_iCurrentMousePositionY);

		int iMouseDeltaX = m_iCurrentMousePositionX - m_iLastMousePositionX;
		int iMouseDeltaY = m_iCurrentMousePositionY - m_iLastMousePositionY;

		m_iLastMousePositionX = m_iCurrentMousePositionX;
		m_iLastMousePositionY = m_iCurrentMousePositionY;

		m_fRotationVelocityX = iMouseDeltaX * m_fRotationScaler;
		m_fRotationVelocityY = iMouseDeltaY * m_fRotationScaler;

		m_fRotationPitchAngle += m_fRotationVelocityY;
		m_fRotationYawAngle += m_fRotationVelocityX;

		// Limit pitch to straight up or straight down
		m_fRotationPitchAngle = max(-M_PI / 2.0f, m_fRotationPitchAngle);
		m_fRotationPitchAngle = min(M_PI / 2.0f, m_fRotationPitchAngle);

		Math::Matrix4f mRotation;
		Math::MatrixRotationYawPitchRoll(m_fRotationYawAngle, m_fRotationPitchAngle, 0, mRotation);

		// update vectors
		const Math::Vector4f v4LocalUp(0.f, 1.f, 0.f, 0.f);
		const Math::Vector4f v4LocalZ(0.f, 0.f, 1.f, 0.f);
		const Math::Vector4f v4NewUp = mRotation * v4LocalUp;
		const Math::Vector4f v4NewZ = mRotation * v4LocalZ;

		m_v3Lookat = m_v3Position + Math::Vector3f(v4NewZ.x, v4NewZ.y, v4NewZ.z);
		m_v3Up = Math::Vector3f(v4NewUp.x, v4NewUp.y, v4NewUp.z);

	}

	if (m_bTranslate)
	{
		Math::Vector3f v3LookDirection = m_v3Lookat - m_v3Position;
		v3LookDirection.Normalize();
		m_v3Up.Normalize();
		Math::Vector3f v3Left = Math::Cross3f(m_v3Up, v3LookDirection);
		v3Left.Normalize();
		m_v3Position += v3LookDirection * m_fForward * fTimeLapse * m_fVelocity;

		m_v3Position += v3Left * (-m_fLeft) * fTimeLapse * m_fVelocity;
		m_v3Lookat += v3Left * (-m_fLeft) * fTimeLapse * m_fVelocity;
	}
}
//------------------------------------------------------------------old camera code
/*
void Camera::UpdateCamera(float fTimeLapse)
{
// move camera
Math::Vector3f v3LookDirection = m_v3Lookat - m_v3Position;
v3LookDirection.Normalize();
m_v3Position += v3LookDirection * m_fForward * fTimeLapse * m_fVelocity;
m_v3Lookat += v3LookDirection * m_fForward * fTimeLapse * m_fVelocity;

Math::Vector3f v3Left;

if (abs(1.f - abs(v3LookDirection.y)) < 0.000001f)
v3Left = Math::Vector3f(-1.f, 0.f, 0.f);
else
{
v3Left = Cross3f(v3LookDirection, m_v3Up);
v3Left.Normalize();
}


m_v3Position += v3Left * m_fLeft * fTimeLapse* m_fVelocity;
m_v3Lookat += v3Left * m_fLeft * fTimeLapse * m_fVelocity;

if (m_iRotateX == 0 && m_iRotateY == 0)
return;

// rotated camera direction
Math::Vector3f v3YDirection = Cross3f(v3Left, v3LookDirection);
v3YDirection.Normalize();

// in view space
Math::Vector4f v4OldDirection = Math::Vector4f(0, 0, 1, 0);

float fAngle;

// rotate around y axis in view space
fAngle = m_fRotationFactor * m_iRotateX;
//if (fAngle > 5)
//	fAngle = 5;
//if (fAngle < -5)
//	fAngle = -5;
Math::Matrix4f rotateY(cosf(fAngle/180.f*M_PI), 0.f, -sinf(fAngle/180.f*M_PI), 0.f,
0.f, 1.f, 0.f, 0.f,
sinf(fAngle/180.f*M_PI), 0.f, cosf(fAngle/180.f*M_PI), 0.f,
0.f, 0.f, 0.f, 1.f);
v4OldDirection = rotateY * v4OldDirection;

// rotate around x axis in view space
fAngle = m_fRotationFactor * m_iRotateY;
//if (fAngle > 5)
//	fAngle = 5;
//if (fAngle < -5)
//	fAngle = -5;
Math::Matrix4f rotateX(1.f, 0.f, 0.f, 0.f,
0.f, cosf(fAngle/180.f*M_PI), sinf(fAngle/180.f*M_PI), 0.f,
0.f, -sinf(fAngle/180.f*M_PI), cosf(fAngle/180.f*M_PI), 0.f,
0.f, 0.f, 0.f, 1.f);
v4OldDirection = rotateX * v4OldDirection;

// v3OldDirection is z axis in view space
Math::Vector3f v3OldDirection(v4OldDirection.x, v4OldDirection.y, v4OldDirection.z);

Math::Vector3f v3NewDirection;

Math::Vector3f v3Right = -v3Left;
v3NewDirection.x = v3Right.x * v3OldDirection.x + v3YDirection.x * v3OldDirection.y + v3LookDirection.x * v3OldDirection.z;
v3NewDirection.y = v3Right.y * v3OldDirection.x + v3YDirection.y * v3OldDirection.y + v3LookDirection.y * v3OldDirection.z;
v3NewDirection.z = v3Right.z * v3OldDirection.x + v3YDirection.z * v3OldDirection.y + v3LookDirection.z * v3OldDirection.z;
v3NewDirection.Normalize();

m_v3Lookat = m_v3Position + v3NewDirection * 1.0f;		// new lookat point

// clean movement parameters
m_fForward = 0.f;
m_fLeft = 0.f;
m_iRotateX = 0;
m_iRotateY = 0;
}
*/


void Camera::SetRenderObject(Scene::IRenderObject* pRenderObject)
{
	m_pRenderObject = pRenderObject;
}

IRenderObject* Camera::GetRenderObject()
{
	return m_pRenderObject;
}

EntityType Camera::GetType() const
{
	return ENTITY_CAMERA;
}

bool Camera::IsVisible() const
{
	return m_bVisible;
}

void Camera::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

bool Camera::IsDrawable() const
{
	return m_bDrawable;
}

bool Camera::IsActive() const
{
	return m_bActive;
}

void Camera::SetActive(bool bActive)
{
	m_bActive = bActive;
}

void Camera::ViewFrustumCulling(const Math::Frustum& frustum)
{

}

void Camera::SetLocalToWorld(const Math::Matrix4f& mWorld)
{

}

//-------------------------------------------------------------------
Math::Quaternion Camera::GetQuaternionFromArcballPoints(const Math::Vector3f& v3From, const Math::Vector3f& v3To)
{
	float fDot = Math::Dot3f(v3From, v3To);
	Math::Vector3f v3Cross = Math::Cross3f(v3From, v3To);

	return Math::Quaternion(v3Cross.x, v3Cross.y, v3Cross.z, fDot);
}

//-------------------------------------------------------------------
Math::Vector3f Camera::GetScreenToVector(float fScreenX, float fScreenY)
{
	// Scale to screen
	float x = -(fScreenX - m_fWindowOffsetX - m_fWindowWidth / 2.f) / (m_fArcballRadius * m_fWindowWidth / 2.f);
	float y = (fScreenY - m_fWindowOffsetY - m_fWindowHeight / 2.f) / (m_fArcballRadius * m_fWindowHeight / 2.f);

	float z = 0.0f;
	float mag = x * x + y * y;

	if (mag > 1.0f)
	{
		float scale = 1.0f / sqrtf(mag);
		x *= scale;
		y *= scale;
	}
	else
		z = sqrtf(1.0f - mag);

	// Return vector
	return Math::Vector3f(x, y, z);
}
} // namespace Scene
} // namespace Magnet