#ifndef IRENDEROBJECT_H
#define IRENDEROBJECT_H

#include "Math\Vector3f.h"
#include "Math\Math.h"
#include "Math\Matrix4f.h"
#include "Math\AABB.h"
#include "Math\Sphere.h"

namespace Magnet
{
namespace Scene
{

class Surface;

enum RenderObjectType
{
	RENDEROBJECT_NORMAL,
	RENDEROBJECT_SUN,
	RENDEROBJECT_OPAQUE
};

class IRenderObject
{
public:
	IRenderObject()
	{
		m_v3Scale = Math::Vector3f(1.f, 1.f, 1.f);
	}
	virtual ~IRenderObject(){}

	virtual void Initialize() = 0;
	virtual RenderObjectType GetType() const = 0;
	virtual bool IsInitialized() const = 0;
	virtual void AddSurface(Surface* pSurface) = 0;

	void SetTranslation(const Math::Vector3f& v3Translation);
	void SetScale(const Math::Vector3f& v3Scale);
	void SetRotation(const Math::Vector3f& v3Rotation);

	void GetLocalToWorldTransformation(Math::Matrix4f& mLocalToWorld);

protected:
	Math::Vector3f m_v3Translation;
	Math::Vector3f m_v3Scale;
	Math::Vector3f m_v3Rotation;

};

inline void IRenderObject::SetTranslation(const Math::Vector3f& v3Translation)
{
	m_v3Translation = v3Translation;
}

inline void IRenderObject::SetScale(const Math::Vector3f& v3Scale)
{
	m_v3Scale = v3Scale;
}

inline void IRenderObject::SetRotation(const Math::Vector3f& v3Rotation)
{
	m_v3Rotation = v3Rotation;
}

inline void IRenderObject::GetLocalToWorldTransformation(Math::Matrix4f& mLocalToWorld)
{
	// rotate, scale, translate
	Math::Matrix4f mRotation;

	Math::Matrix4f mScale;
	Math::GetScaleMatrix(m_v3Scale, mScale);

	Math::Matrix4f mTranslation;
	Math::GetTranslationMatrix(m_v3Translation, mTranslation);

	mLocalToWorld = mTranslation * mScale;
}

} // namespace Scene
} // namespace Magnet

#endif
