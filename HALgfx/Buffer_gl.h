#ifndef GL_BUFFER_H
#define GL_BUFFER_H

#include <glew.h>		// has to be included before gl.h
#include <gl/gl.h>
#include <gl/glu.h>

#include "IBuffer.h"
#include "Resource_gl.h"


namespace Magnet
{
namespace HALgfx
{

class GLBuffer : public IBuffer
{
public:
	GLBuffer(const BufferDesc& desc) : mDesc(desc)
	{
	}
	~GLBuffer()
	{}

	virtual void GetDesc(BufferDesc& bufferDesc) const;
	virtual ResourceType GetType() const;
	//virtual unsigned int GetElementCount() const;
	//virtual unsigned int GetElementStride() const;
	void Create(const BufferDesc& bufferDesc, const SubResourceData& pSubResourceData);
	GLuint GetGLHandle();

private:
	BufferDesc mDesc;
	unsigned int m_uElemCount;
	GLuint m_buffer;
};

inline GLuint GLBuffer::GetGLHandle()
{
	return m_buffer;
}

inline ResourceType GLBuffer::GetType() const
{
	return RESOURCE_BUFFER;
}

} // namespace HALgfx
} // namespace Magnet

#endif