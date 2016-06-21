#ifndef GL_RESOURCE_H
#define GL_RESOURCE_H

#include <glew.h>		// has to be included before gl.h
#include <gl/gl.h>
#include <gl/glu.h>

#include "IResource.h"

namespace Magnet
{
namespace HALgfx
{
GLenum GetGLUsage(Usage usage);
GLenum GetGLBindFlags(unsigned int bindFlags);
GLenum GetGLAccessFlags(unsigned int flags);
GLenum GetGLMiscFlags(unsigned int flags);

} // namespace HALgfx
} // namespace Magnet

#endif