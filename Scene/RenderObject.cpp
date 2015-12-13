#include "RenderObject.h"

namespace Magnet
{
namespace Scene
{
RenderObject::RenderObject()
{

}

RenderObject::~RenderObject()
{

}

void RenderObject::Initialize()
{

}

RenderObjectType RenderObject::GetType() const
{
	return RENDEROBJECT_NORMAL;
}

bool RenderObject::IsInitialized() const
{
	return m_bInitialized;
}

void RenderObject::AddSurface(Surface* pSurface)
{
	m_lSurfaceList.push_back(pSurface);
}

const std::list<Surface*>& RenderObject::GetSurfaceList() const
{
	return m_lSurfaceList;
}

} // namespace Scene
} // namespace Magnet