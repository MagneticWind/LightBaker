#ifndef GL_VERTEX_SHADER_H
#define GL_VERTEX_SHADER_H

#include "glew.h"		// has to be included before gl.h
#include <gl/gl.h>
#include <gl/glu.h>

#include "IShader.h"

namespace Magnet
{
namespace HALgfx
{
class GLVertexShader : public IShader
{
public:
	GLVertexShader();
	~GLVertexShader();
	void Create(const void* pShaderSource, int iSize);

	const GLuint GetGLId() const;
private:
	GLuint m_shader;
};
}
}

#endif