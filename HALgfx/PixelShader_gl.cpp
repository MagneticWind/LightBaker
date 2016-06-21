#include "PixelShader_gl.h"

namespace Magnet
{
namespace HALgfx
{
GLPixelShader::GLPixelShader()
{}

GLPixelShader::~GLPixelShader()
{}

void GLPixelShader::Create(const void* pShaderSource, int iSize)
{
	m_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_shader, 1, (const GLchar**)&pShaderSource, 0);
	glCompileShader(m_shader);
}

const GLuint GLPixelShader::GetGLId() const
{
	return m_shader;
}

}
}