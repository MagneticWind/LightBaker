#ifndef GL_FORMAT_H
#define GL_FORMAT_H

#include <glew.h>		// has to be included before gl.h
#include <gl/gl.h>
#include <gl/glu.h>

#include "Format.h"

namespace Magnet
{
namespace HALgfx
{
	void GetGLFormat(Format format, GLint& internalFormat, GLenum& glFormat, GLenum& glType);
} // namespace HALgfx
} // namespace Magnet

#endif