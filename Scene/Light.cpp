#include <string.h>
#include "Light.h"

namespace Magnet
{
namespace Scene
{
Light::Light(const char* pName)
{
	strcpy(m_name, pName);

	m_fIntensity = 1.f;

	m_bActive = true;
	m_bVisible = false;
	m_bDrawable = false;
}

//------------------------------------------------------------------
EntityType Light::GetType() const
{
	return ENTITY_LIGHT;
}

//------------------------------------------------------------------
bool Light::IsDrawable() const
{
	return m_bDrawable;
}

//------------------------------------------------------------------
bool Light::IsActive() const
{
	return m_bActive;
}

//------------------------------------------------------------------
bool Light::IsVisible() const
{
	return m_bVisible;
}

//------------------------------------------------------------------
void Light::SetActive(bool bActive)
{
	m_bActive = bActive;
}

//------------------------------------------------------------------
void Light::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

//------------------------------------------------------------------
void Light::SetRenderObject(IRenderObject* pRenderObject)
{
	m_pRenderObject = pRenderObject;
}

//------------------------------------------------------------------
IRenderObject* Light::GetRenderObject()
{
	return m_pRenderObject;
}

//------------------------------------------------------------------
void Light::SetLocalToWorld(const Math::Matrix4f& mWorld)
{
	m_mLocalToWorld = mWorld;
}

//------------------------------------------------------------------
void Light::Update(float fTimeLapse)
{

}

//------------------------------------------------------------------
void Light::ViewFrustumCulling(const Math::Frustum& frustum)
{
}

} // namespace Scene
} // namespace Magnet