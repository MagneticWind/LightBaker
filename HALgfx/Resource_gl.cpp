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
			return GL_STATIC_DRAW;
		default:
			return GL_DYNAMIC_DRAW;
		}
	}

	GLenum GetGLBindFlags(unsigned int bindFlags)
	{
		if (bindFlags & BIND_VERTEX_BUFFER)
		{
			return GL_VERTEX_ARRAY;
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
}
}