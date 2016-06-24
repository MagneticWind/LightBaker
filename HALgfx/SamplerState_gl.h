#ifndef GL_SAMPLER_STATE_H
#define GL_SAMPLER_STATE_H

#include "ISamplerState.h"

#include <glew.h>		// has to be included before gl.h
#include <gl/gl.h>
#include <gl/glu.h>

namespace Magnet
{
namespace HALgfx
{

class GLSamplerState : public ISamplerState
{
public:
	GLSamplerState(const SamplerStateDesc& descSampler);
	~GLSamplerState();

	GLuint GetGLHandle() const;

private:
	GLenum GetGLAddressMode(AddressMode mode);
	GLenum GetGLFilter(Filter filter);

private:
	SamplerStateDesc m_desc;
	GLuint m_sampler;

};

inline GLuint GLSamplerState::GetGLHandle() const
{
	return m_sampler;
}

} // namespace HALgfx
} // namespace Magnet

#endif