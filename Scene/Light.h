#ifndef LIGHT_H
#define LIGTH_H

#include "Math\Vector3f.h"
#include "Math\Matrix4f.h"
#include "IEntity.h"

namespace Magnet
{
namespace Scene
{

enum LightType
{
	LIGHT_NONE,
	LIGHT_DIRECTIONAL,
	LIGHT_POINT,
	LIGHT_SPOT,
	LIGHT_SPHERE,
	LIGHT_SH,
	LIGHT_ENVIRONMENT
};

class Light : public IEntity
{
public:
	Light(const char* pName);

	LightType GetLightType() const;

	void SetType(LightType type);
	void SetColor(const Math::Vector3f& v3Color);
	void SetIntensity(float fIntensity);
	void SetPosition(const Math::Vector3f& v3Position);
	void SetDirection(const Math::Vector3f& v3Direction);
	void SetVisible(bool bVisible);
	void SetCastShadow(bool bCastShadow);
	void SetRadius(float fRadius);
	void SetSpotProperty(float fOuterAngle, float InnerAngle, float FallOff);

	const Math::Vector3f& GetPosition() const;
	const Math::Vector3f& GetColor() const;

	const Math::Vector3f& GetDirection() const;
	float GetIntensity() const;

	float GetOuterAngle() const;
	float GetInnerAngle() const;
	float GetFallOff() const;
	float GetRadius() const;

	bool IsCastShadow() const;

	void SetRenderObject(IRenderObject* pRenderObject);
	IRenderObject* GetRenderObject();
	EntityType GetType() const;

	virtual bool IsDrawable() const;
	virtual bool IsActive() const;
	virtual bool IsVisible() const;
	virtual void SetActive(bool bActive);
	virtual void SetLocalToWorld(const Math::Matrix4f& mWorld);
	virtual void Update(float fTimeLapse);
	virtual void ViewFrustumCulling(const Math::Frustum& frustum);

protected:
	char m_name[256];
	LightType m_eLightType;
	Math::Vector3f m_v3Color;
	Math::Vector3f m_v3Position;     // point light, spot light
	Math::Vector3f m_v3Direction;    // directional light
	float m_fIntensity;
	float m_fOuterAngle;       // spot light
	float m_fInnerAngle;       // spot light
	float m_fFallOff;          // spot light
	float m_fRadius;           // sphere light
	float m_fInfluenceRadius;  // point light, sphere light
	bool m_bCastShadow;

	IRenderObject* m_pRenderObject;
	EntityType m_eType;
	bool m_bVisible;    // if this entity is in view frustum
	bool m_bDrawable;   // if this entity can be drawn, does it have an render object
	bool m_bActive;     // if this entity is updating
	Math::Matrix4f m_mLocalToWorld;
};


//==================================================================
inline LightType Light::GetLightType() const
{
	return m_eLightType;
}

//------------------------------------------------------------------
inline void Light::SetType(LightType type)
{
	m_eLightType = type;
}

//------------------------------------------------------------------
inline void Light::SetColor(const Math::Vector3f& v3Color)
{
	m_v3Color = v3Color;
}

//------------------------------------------------------------------
inline void Light::SetIntensity(float fIntensity)
{
	m_fIntensity = fIntensity;
	m_v3Color = m_v3Color * fIntensity;
}

//------------------------------------------------------------------
inline void Light::SetPosition(const Math::Vector3f& v3Position)
{
	m_v3Position = v3Position;
}

//------------------------------------------------------------------
inline void Light::SetDirection(const Math::Vector3f& v3Direction)
{
	m_v3Direction = v3Direction;
}

//------------------------------------------------------------------
inline void Light::SetCastShadow(bool bCastShadow)
{
	m_bCastShadow = bCastShadow;
}

//------------------------------------------------------------------
inline void Light::SetRadius(float fRadius)
{
	m_fRadius = m_fRadius;
}

//------------------------------------------------------------------
inline void Light::SetSpotProperty(float fOuterAngle, float fInnerAngle, float fFallOff)
{
	m_fOuterAngle = fOuterAngle;
	m_fInnerAngle = fInnerAngle;
	m_fFallOff = fFallOff;
}

//------------------------------------------------------------------
inline const Math::Vector3f& Light::GetPosition() const
{
	return m_v3Position;
}

//------------------------------------------------------------------
inline const Math::Vector3f& Light::GetColor() const
{
	return m_v3Color;
}

//------------------------------------------------------------------
inline const Math::Vector3f& Light::GetDirection() const
{
	return m_v3Direction;
}

//------------------------------------------------------------------
inline float Light::GetIntensity() const
{
	return m_fIntensity;
}

//------------------------------------------------------------------
inline float Light::GetOuterAngle() const
{
	return m_fOuterAngle;
}

//------------------------------------------------------------------
inline float Light::GetInnerAngle() const
{
	return m_fInnerAngle;
}

//------------------------------------------------------------------
inline float Light::GetFallOff() const
{
	return m_fFallOff;
}

//------------------------------------------------------------------
inline float Light::GetRadius() const
{
	return m_fRadius;
}

//------------------------------------------------------------------
inline bool Light::IsCastShadow() const
{
	return m_bCastShadow;
}

} // namespace Scene
} // namespace Magnet
#endif