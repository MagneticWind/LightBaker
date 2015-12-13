#ifndef ENTITY_H
#define ENTITY_H

#include "IEntity.h"
#include "Math\AABB.h"
#include "Math\Frustum.h"
#include "Math\Matrix4f.h"

namespace Magnet
{

namespace Math
{
class Sphere;
}

namespace Scene
{
class IRenderObject;
class Entity : public IEntity
{
public:
	Entity(const char* pName);

public:
	virtual void SetRenderObject(IRenderObject* pRenderObject);
	virtual IRenderObject* GetRenderObject();
	virtual EntityType GetType() const;
	virtual bool IsVisible() const;
	virtual void SetVisible(bool bVisible);
	virtual bool IsDrawable() const;
	virtual bool IsActive() const;
	virtual void SetActive(bool bActive);
	virtual void Update(float fTimeLapse);
	virtual void ViewFrustumCulling(const Math::Frustum& frustum);
	//virtual void UpdateRenderPass(Render::PassType eType, const Math::Frustum& frustum, const Math::Matrix4f& mView, const Math::Matrix4f& mProjection, const Math::Vector3f& v3EyePosition);
	virtual void SetLocalToWorld(const Math::Matrix4f& mat);

	const Math::AABB& GetBBox() const;
	const Math::Sphere& GetBSphere() const;

private:
	void UpdateInfluenceLights();

private:
	char m_name[256];
	IRenderObject* m_pRenderObject;
	EntityType m_eType;
	bool m_bVisible;    // if this entity is in view frustum
	bool m_bDrawable;   // if this entity can be drawn, does it have an render object
	bool m_bActive;     // if this entity is updating
	Math::Matrix4f m_mLocalToWorld;
};

} // namespace Scene
} // namespace Magnet
#endif