#ifndef GL_COMPARISON_FUNC_H
#define GL_COMPARISON_FUNC_H

#include <glew.h>		// has to be included before gl.h
#include <gl/gl.h>
#include <gl/glu.h>

#include "ComparisonFunc.h"

namespace Magnet
{
namespace HALgfx
{
	GLenum GetGLComparisonFunc(ComparisonFunc func);
} // namespace HALgfx
} // namespace Magnet

#endif