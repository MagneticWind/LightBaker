#ifndef GL_RESOURCE_H
#define GL_RESOURCE_H

#include <glew.h>		// has to be included before gl.h
#include <gl/gl.h>
#include <gl/glu.h>

#include "IResource.h"
#include "ITexture1d.h"
#include "ITexture2d.h"

namespace Magnet
{
namespace HALgfx
{
GLenum GetGLUsage(Usage usage);
GLenum GetGLBindFlags(unsigned int bindFlags);
GLenum GetGLAccessFlags(unsigned int flags);
GLenum GetGLMiscFlags(unsigned int flags);
GLenum GetGLMapType(MapType type);

struct GLTexture1dDesc
{

};

struct GLTexture2dDesc
{

};

void GetGLTexture1dDesc(const Texture1dDesc& desc, GLTexture1dDesc glDesc);
void GetGLTexture2dDesc(const Texture2dDesc& desc, GLTexture2dDesc glDesc);

} // namespace HALgfx
} // namespace Magnet

#endif