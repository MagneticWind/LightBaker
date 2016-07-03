#include "Resource_gl.h"

namespace Magnet
{
namespace HALgfx
{
GLenum GetGLUsage(Usage usage)
{
	switch (usage)
	{
	case USAGE_DYNAMIC:
		return GL_DYNAMIC_DRAW;
	default:
		return GL_STATIC_DRAW;
	}
}

GLenum GetGLBindFlags(unsigned int bindFlags)
{
	if (bindFlags & BIND_VERTEX_BUFFER)
	{
		return GL_ARRAY_BUFFER;
	}
	else if (bindFlags & BIND_INDEX_BUFFER)
	{
		return GL_ELEMENT_ARRAY_BUFFER;
	}
	else if (bindFlags & BIND_CONSTANT_BUFFER)
	{
		return GL_UNIFORM_BUFFER;
	}
}

GLenum GetGLMapType(MapType type)
{
	switch (type)
	{
	case MAP_READ:
		return GL_READ_ONLY;
	case MAP_READ_WRITE:
		return GL_READ_WRITE;
	case MAP_WRITE:
		return GL_WRITE_ONLY;
	case MAP_WRITE_DISCARD:
		return GL_WRITE_ONLY;
	}
}

} // namespace HALgfx
} // namespace Magnet