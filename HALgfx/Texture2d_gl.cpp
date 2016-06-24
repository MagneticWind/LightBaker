#include "glew.h"		// has to be included before gl.h
#include <gl/gl.h>
#include <gl/glu.h>

#include "Format_gl.h"
#include "Texture2d_gl.h"

namespace Magnet
{
namespace HALgfx
{
	GLTexture2d::GLTexture2d()
	{

	}

	GLTexture2d::~GLTexture2d()
	{

	}

	void GLTexture2d::Create(const Texture2dDesc& texture2dDesc, const SubResourceData& subResourceData)
	{
		glGenTextures(1, &m_uTexture);
		glBindTexture(GL_TEXTURE_2D, m_uTexture);

		GLint internalFormat;
		GLenum format;
		GLenum type;
		GetGLFormat(texture2dDesc.format, internalFormat, format, type);

		glTexImage2D(GL_TEXTURE_2D, texture2dDesc.mipLevels, internalFormat, texture2dDesc.width, texture2dDesc.height, 0,
			format, type, subResourceData.pMem);
	}

} // namespace HALgfx
} // namespace Magnet