#ifndef CAMERA_H
#define CAMERA_H

#include "IEntity.h"

#include "Math\Frustum.h"
#include "Math\Quaternion.h"
#include "Math\Matrix4f.h"
#include "Math\Vector3f.h"

namespace Magnet
{

namespace Scene
{

typedef void(*CallbackGetCurrentMousePosition) (int& iPositionX, int& iPositionY);

class Camera : public IEntity
{

public:
	Camera(const char* pName);
	~Camera();

	void SetPosition(const Math::Vector3f& v3Position);
	void SetDirection(const Math::Vector3f& v3Direction);
	void SetUpVector(const Math::Vector3f& v3Up);
	void SetAspectRatio(const float fAspectRatio);
	void SetFOV(const float fFOV);
	void SetLookat(const Math::Vector3f& v3Lookat);
	void SetNear(float fZNear);
	void SetFar(float fZFar);
	void SetVelocity(float fVelocity);
	void SetRotationFactor(float fRotationFactor);

	const Math::Vector3f& GetPosition() const;
	const Math::Vector3f& GetDirection() const;
	const Math::Vector3f& GetUpVector() const;
	const Math::Vector3f& GetLookat() const;
	float GetAspectRatio() const;
	float GetFOV() const;
	float GetNear() const;
	float GetFar() const;
	const Math::Frustum& GetViewFrustum() const;

	EntityType GetEntityType() const;
	const Math::Matrix4f& GetViewMatrix() const;
	const Math::Matrix4f& GetProjectionMatrix() const;

	void SetForward(float fForward);
	void SetLeft(float fLeft);
	void SetRotateX(int iRotateX);
	void SetRotateY(int iRotateY);

	// arcball related, used for model view camera
	Math::Quaternion GetQuaternionFromArcballPoints(const Math::Vector3f& v3From, const Math::Vector3f& v3To);
	Math::Vector3f GetScreenToVector(float fScreenX, float fScreenY);

	void SetGetCurrentMousePositionCallback(CallbackGetCurrentMousePosition callbackGetMousePosition);
	void SetRotationParams(bool bRotate, int iMousePositionX, int iMousePositionY);
	void SetTranslationParams(bool bTranslate, int iMoveForward, int iMoveLeft);

public:
	virtual void SetRenderObject(IRenderObject* pRenderObject);
	virtual IRenderObject* GetRenderObject();
	virtual EntityType GetType() const;
	virtual bool IsVisible() const;
	virtual void SetVisible(bool bVisible);
	virtual void SetLocalToWorld(const Math::Matrix4f& mWorld);
	virtual bool IsDrawable() const;
	virtual bool IsActive() const;
	virtual void SetActive(bool bActive);
	virtual void Update(float fTimeLapse);
	virtual void ViewFrustumCulling(const Math::Frustum& frustum);
	//virtual void UpdateRenderPass(Render::PassType eType, const Math::Frustum& frustum, const Math::Matrix4f& mView, const Math::Matrix4f& mProjection, const Math::Vector3f& v3EyePosition);

private:
	void UpdateViewFrustum();
	void UpdateInput(float fTimeLapse);
	void UpdateViewProjectMatrices();

private:
	Math::Vector3f m_v3Position;
	Math::Vector3f m_v3Direction;
	Math::Vector3f m_v3Up;
	Math::Vector3f m_v3Lookat;
	float    m_fAspectRatio;    // aspection ratio of view
	float    m_fFOV;            // field of view, in degrees
	float    m_fZNear;
	float    m_fZFar;

	bool m_bIsPerspective;
	bool m_bIsInitialized;
	Math::Frustum m_frustum;
	Math::Matrix4f m_mView;
	Math::Matrix4f m_mProjection;

	// arcball related, used for model view camera
	Math::Vector3f m_v3DownPoint;           // starting point of rotation arc
	Math::Vector4f m_v3CurrentPoint;        // current point of rotation arc
	Math::Quaternion mArcballQuaternionDown;
	Math::Quaternion mArcballQuaternionNow;
	float m_fArcballLastMouseX;                    // screen x of last mouse point
	float m_fArcballLastMouthY;                    // screen y of last mouse point
	float m_fArcballRadius;
	float m_fWindowOffsetX;
	float m_fWindowOffsetY;
	float m_fWindowWidth;
	float m_fWindowHeight;
	bool m_bDragArcball;

	float m_fVelocity;

	// camera movement
	float m_fForward;
	float m_fLeft;
	int m_iRotateX;
	int m_iRotateY;

	CallbackGetCurrentMousePosition m_callbackGetCurrentMousePosition;
	bool m_bRotate;
	bool m_bTranslate;
	int m_iLastMousePositionX;
	int m_iLastMousePositionY;
	int m_iCurrentMousePositionX;
	int m_iCurrentMousePositionY;
	float m_fRotationVelocityX;
	float m_fRotationVelocityY;
	float m_fRotationScaler;
	float m_fRotationPitchAngle;
	float m_fRotationYawAngle;
	float m_fRotationRollAngle;

	IRenderObject* m_pRenderObject;
	EntityType m_eType;
	bool m_bVisible;    // if this entity is in view frustum
	bool m_bDrawable;   // if this entity can be drawn, does it have an render object
	bool m_bActive;     // if this entity is updating
	Math::Matrix4f m_mLocalToWorld;
};

//------------------------------------------------------------------
inline void Camera::SetPosition(const Math::Vector3f& v3Position)
{
	m_v3Position = v3Position;
}

//------------------------------------------------------------------
inline void Camera::SetDirection(const Math::Vector3f& v3Direction)
{
	m_v3Direction = v3Direction;
}

//------------------------------------------------------------------
inline void Camera::SetUpVector(const Math::Vector3f& v3Up)
{
	m_v3Up = v3Up;
}

//------------------------------------------------------------------
inline void Camera::SetAspectRatio(const float fAspectRatio)
{
	m_fAspectRatio = fAspectRatio;
}

//------------------------------------------------------------------
inline void Camera::SetFOV(const float fFOV)
{
	m_fFOV = fFOV;
}

//------------------------------------------------------------------
inline void Camera::SetLookat(const Math::Vector3f& v3Lookat)
{
	m_v3Lookat = v3Lookat;
}

//------------------------------------------------------------------
inline void Camera::SetNear(float fZNear)
{
	m_fZNear = fZNear;
}

//------------------------------------------------------------------
inline void Camera::SetFar(float fZFar)
{
	m_fZFar = fZFar;
}

//------------------------------------------------------------------
inline void Camera::SetVelocity(float fVelocity)
{
	m_fVelocity = fVelocity;
}

//------------------------------------------------------------------
inline void Camera::SetRotationFactor(float fRotationFactor)
{
	m_fRotationScaler = fRotationFactor;
}


//------------------------------------------------------------------
inline const Math::Vector3f& Camera::GetPosition() const
{
	return m_v3Position;
}

//------------------------------------------------------------------
inline const Math::Vector3f& Camera::GetDirection() const
{
	return m_v3Direction;
}

//------------------------------------------------------------------
inline const Math::Vector3f& Camera::GetUpVector() const
{
	return m_v3Up;
}

//------------------------------------------------------------------
inline const Math::Vector3f& Camera::GetLookat() const
{
	return m_v3Lookat;
}

//------------------------------------------------------------------
inline float Camera::GetAspectRatio() const
{
	return m_fAspectRatio;
}

//------------------------------------------------------------------
inline float Camera::GetFOV() const
{
	return m_fFOV;
}

//------------------------------------------------------------------
inline float Camera::GetNear() const
{
	return m_fZNear;
}

//------------------------------------------------------------------
inline float Camera::GetFar() const
{
	return m_fZFar;
}

//------------------------------------------------------------------
inline const Math::Frustum& Camera::GetViewFrustum() const
{
	return m_frustum;
}

//------------------------------------------------------------------
inline const Math::Matrix4f& Camera::GetViewMatrix() const
{
	return m_mView;
}

//-------------------------------------------------------------------
inline const Math::Matrix4f& Camera::GetProjectionMatrix() const
{
	return m_mProjection;
}

//-------------------------------------------------------------------
inline void Camera::SetForward(float fForward)
{
	m_fForward = fForward;
}

//-------------------------------------------------------------------
inline void Camera::SetLeft(float fLeft)
{
	m_fLeft = fLeft;
}

//-------------------------------------------------------------------
inline void Camera::SetRotateX(int iRotateX)
{
	m_iRotateX = iRotateX;
}

//-------------------------------------------------------------------
inline void Camera::SetRotateY(int iRotateY)
{
	m_iRotateY = iRotateY;
}

//-------------------------------------------------------------------
inline void Camera::SetGetCurrentMousePositionCallback(CallbackGetCurrentMousePosition callbackGetMousePosition)
{
	m_callbackGetCurrentMousePosition = callbackGetMousePosition;
}

} // namespace Scene
} // namespace Magnet

#endif