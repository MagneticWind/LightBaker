#ifndef IENTITY_H
#define IENTITY_H

namespace Magnet
{
namespace Math
{
class Frustum;
class Matrix4f;
class Vector3f;
}

namespace Scene
{
enum EntityType
{
	ENTITY_NORMAL,
	ENTITY_LIGHT,
	ENTITY_CAMERA
};

class IRenderObject;

class IEntity
{
public:
	virtual ~IEntity(){}
	virtual void SetRenderObject(IRenderObject* pRenderObject) = 0;
	virtual IRenderObject* GetRenderObject() = 0;
	virtual EntityType GetType() const = 0;
	virtual bool IsVisible() const = 0;
	virtual void SetVisible(bool bVisible) = 0;
	virtual void SetLocalToWorld(const Math::Matrix4f& mWorld) = 0;
	virtual bool IsDrawable() const = 0;
	virtual bool IsActive() const = 0;
	virtual void SetActive(bool bActive) = 0;
	virtual void Update(float fTimeLapse) = 0;
	virtual void ViewFrustumCulling(const Math::Frustum& frustum) = 0;
};

} // namespace Scene
} // namespace Magnet

#endif