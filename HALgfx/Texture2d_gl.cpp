#include <assert.h>

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

	void GLTexture2d::Create(const Texture2dDesc& texture2dDesc, const SubResourceData* subResourceData)
	{
		if (texture2dDesc.arraySize == 6)
		{
			glGenTextures(1, &m_uTexture);
			glActiveTexture(GL_TEXTURE0);

			glBindTexture(GL_TEXTURE_CUBE_MAP, m_uTexture);

			GLint internalFormat;
			GLenum format;
			GLenum type;
			GetGLFormat(texture2dDesc.format, internalFormat, format, type);

			for (GLuint i = 0; i < 6; i++)
			{
				// support hdr cubemap only
				if (texture2dDesc.format == HALgfx::FORMAT_R32G32B32A32_FLOAT)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
						internalFormat, texture2dDesc.width, texture2dDesc.height, 0, format, type, subResourceData[i].pMem);

					GLenum err = glGetError();
					if (err != GL_NO_ERROR)
					{
						assert(0);
					}
				}
				else
				{
					assert(0);
				}
			}

			// sampler is bound to cubemap
			/*glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);*/

			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
		else
		{
			glGenTextures(1, &m_uTexture);
			glBindTexture(GL_TEXTURE_2D, m_uTexture);

			GLint internalFormat;
			GLenum format;
			GLenum type;
			GetGLFormat(texture2dDesc.format, internalFormat, format, type);

			glTexImage2D(GL_TEXTURE_2D, texture2dDesc.mipLevels, internalFormat, texture2dDesc.width, texture2dDesc.height, 0,
				format, type, subResourceData->pMem);

			GLenum err = glGetError();
			if (err != GL_NO_ERROR)
			{
				assert(0);
			}
		}
	}

} // namespace HALgfx
} // namespace Magnet