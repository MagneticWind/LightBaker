#include <assert.h>
#include "Format_gl.h"

namespace Magnet
{
	namespace HALgfx
	{
		void GetGLFormat(Format format, GLint& glInternalFormat, GLenum& glFormat, GLenum& glType)
		{
			switch (format)
			{
			case FORMAT_R32G32B32A32_FLOAT:
				glInternalFormat = GL_RGBA32F;
				glFormat = GL_RGBA;
				glType = GL_FLOAT;
				break;
			case FORMAT_R32G32B32_FLOAT:
				glInternalFormat = GL_RGB32F;
				glFormat = GL_RGB;
				glType = GL_FLOAT;
				break;
			case FORMAT_R16G16B16A16_FLOAT:
				glInternalFormat = GL_RGBA16F;
				glFormat = GL_RGBA;
				glType = GL_HALF_FLOAT;
				break;
			case FORMAT_R8G8B8A8_UINT:
				glInternalFormat = GL_RGBA8UI;
				glFormat = GL_RGBA;
				glType = GL_UNSIGNED_INT;
				break;
			case FORMAT_R8G8B8A8_UNORM:
				glInternalFormat = GL_RGBA8;
				glFormat = GL_RGBA;
				glType = GL_BYTE;
				break;
			case FORMAT_R32_TYPELESS:
				glInternalFormat = GL_R32F;
				glFormat = GL_RED;
				glType = GL_FLOAT;
				break;
			case FORMAT_R32G32_FLOAT:
				glInternalFormat = GL_RG32F;
				glFormat = GL_RG;
				glType = GL_FLOAT;
				break;
			case FORMAT_D32_FLOAT:
				glInternalFormat = GL_R32F;
				glFormat = GL_DEPTH_COMPONENT;
				glType = GL_FLOAT;
				break;
			case FORMAT_R32_FLOAT:
				glInternalFormat = GL_R32F;
				glFormat = GL_RED;
				glType = GL_FLOAT;
				break;
			case FORMAT_R32_UINT:
				glInternalFormat = GL_R32UI;
				glFormat = GL_RED;
				glType = GL_UNSIGNED_INT;
				break;
			case FORMAT_D32_FLOAT_S8X24_UINT:
			case FORMAT_R24G8_TYPELESS:
			case FORMAT_D24_UNORM_S8_UINT:
			case FORMAT_R24_UNORM_X8_TYPELESS:
			case FORMAT_UNKNOWN:
			default:
			{
				assert(0);
			}
			}
		}
	}
}