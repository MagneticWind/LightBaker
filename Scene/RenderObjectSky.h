#ifndef RENDEROBJECT_SKY_H
#define RENDEROBJECT_SKY_H

#include <list>

#include "IRenderObject.h"

namespace Magnet
{
namespace Scene
{
class Surface;

class RenderObjectSky : public IRenderObject
{
public:
	RenderObjectSky();
	~RenderObjectSky();

	virtual void Initialize();
	virtual RenderObjectType GetType() const;
	virtual bool IsInitialized() const;
	virtual void AddSurface(Surface* pSurface);

	const std::list<Surface*>& GetSurfaceList() const;

private:
	Math::Matrix4f m_mTransform;
	std::list<Surface*> m_lSurfaceList;

	bool m_bInitialized;
};
} // namespace Scene
} // namespace Magnet
#endif