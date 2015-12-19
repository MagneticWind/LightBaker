#include "RenderObjectSky.h"

namespace Magnet
{
namespace Scene
{
RenderObjectSky::RenderObjectSky()
{

}

RenderObjectSky::~RenderObjectSky()
{

}

void RenderObjectSky::Initialize()
{

}

RenderObjectType RenderObjectSky::GetType() const
{
	return RENDEROBJECT_SKY;
}

bool RenderObjectSky::IsInitialized() const
{
	return m_bInitialized;
}

void RenderObjectSky::AddSurface(Surface* pSurface)
{
	m_lSurfaceList.push_back(pSurface);
}

const std::list<Surface*>& RenderObjectSky::GetSurfaceList() const
{
	return m_lSurfaceList;
}

} // namespace Scene
} // namespace Magnet