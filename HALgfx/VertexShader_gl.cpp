#include "VertexShader_gl.h"

namespace Magnet
{
namespace HALgfx
{
GLVertexShader::GLVertexShader()
{}

GLVertexShader::~GLVertexShader()
{}

void GLVertexShader::Create(const void* pShaderSource, int iSize)
{
	m_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_shader, 1, (const GLchar**)&pShaderSource, 0);
	glCompileShader(m_shader);
}

const GLuint GLVertexShader::GetGLId() const
{
	return m_shader;
}

}
}