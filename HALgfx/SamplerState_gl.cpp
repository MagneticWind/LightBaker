#include "ComparisonFunc_gl.h"
#include "SamplerState_gl.h"

namespace Magnet
{
namespace HALgfx
{

GLSamplerState::GLSamplerState(const SamplerStateDesc& descSampler)
{
	m_desc = descSampler;

	glGenSamplers(1, &m_sampler);

	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, GetGLAddressMode(descSampler.addressU));
	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, GetGLAddressMode(descSampler.addressV));

	glSamplerParameteri(m_sampler, GL_TEXTURE_COMPARE_FUNC, GetGLComparisonFunc(descSampler.comparisonFunc));

	switch (descSampler.filter)
	{
	case FILTER_MIN_MAG_MIP_POINT:
		glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		break;
	case FILTER_MIN_MAG_MIP_LINEAR:
		glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		break;
	case FILTER_ANISOTROPIC:
		glSamplerParameterf(m_sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, descSampler.maxAnisotropy);
		break;
	}
	
	// lodbias, lodmin, lodmax
	//glSamplerParameteri(m_sampler, GL_TEXTURE_COMPARE_FUNC, GetGLComparisonFunc(descSampler.comparisonFunc));

}

GLSamplerState::~GLSamplerState()
{
	glDeleteSamplers(1, &m_sampler);
}

GLenum GLSamplerState::GetGLAddressMode(AddressMode mode)
{
	switch (mode)
	{
	case ADDRESSMODE_WRAP:
		return GL_REPEAT;
	case ADDRESSMODE_CLAMP:
		return GL_CLAMP_TO_EDGE;
	case ADDRESSMODE_BORDER:
		return GL_CLAMP_TO_BORDER;
	}
}


} // namespace HALgfx
} // namespace Magnet
