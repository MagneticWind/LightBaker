#include <assert.h>
#include "ComparisonFunc_gl.h"

namespace Magnet
{
namespace HALgfx
{

	GLenum GetGLComparisonFunc(ComparisonFunc func)
	{
		switch (func)
		{
		case COMPARISON_NEVER:
			return GL_NEVER;
		case COMPARISON_LESS:
			return GL_LESS;
		case COMPARISON_EQUAL:
			return GL_EQUAL;
		case COMPARISON_GREATER:
			return GL_GREATER;
		case COMPARISON_NOT_EQUAL:
			return GL_NOTEQUAL;
		case COMPARISON_GREATER_EQUAL:
			return GL_GEQUAL;
		case COMPARISON_ALWAYS:
			return GL_ALWAYS;
		case COMPARISON_LESS_EQUAL:
			return GL_LEQUAL;
		default:
			assert(0);
		}
	}

} // namespace HALgfx
} // namespace Magnet