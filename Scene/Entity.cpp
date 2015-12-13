#include <string.h>

#include "Entity.h"
#include "IRenderObject.h"

namespace Magnet
{
namespace Scene
{
Entity::Entity(const char* pName)
{
	strcpy(m_name, pName);

	m_bActive = true;
	m_bDrawable = true;
	m_bVisible = true;
}

void Entity::SetRenderObject(IRenderObject* pRenderObject)
{
	m_pRenderObject = pRenderObject;
}

IRenderObject* Entity::GetRenderObject()
{
	return m_pRenderObject;
}

EntityType Entity::GetType() const
{
	return ENTITY_NORMAL;
}

bool Entity::IsVisible() const
{
	return m_bVisible;
}

bool Entity::IsDrawable() const
{
	return m_bDrawable;
}

bool Entity::IsActive() const
{
	return m_bActive;
}

void Entity::SetActive(bool bActive)
{
	m_bActive = bActive;
}

void Entity::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

void Entity::Update(float fTimeLapse)
{
	if (m_pRenderObject->IsInitialized() == false)
		m_pRenderObject->Initialize();
}

void Entity::ViewFrustumCulling(const Math::Frustum& frustum)
{

}

void Entity::SetLocalToWorld(const Math::Matrix4f& mat)
{
	m_mLocalToWorld = mat;
}

//void Entity::UpdateRenderPass(Render::PassType eType, const Math::Frustum& frustum, const Math::Matrix4f& mView, const Math::Matrix4f& mProjection, const Math::Vector3f& v3EyePosition)
//{
//	m_pRenderObject->SetupShaderNodes(eType, frustum, mView, mProjection, v3EyePosition);
//}

} // namespace Scene
} // namespace Magnet