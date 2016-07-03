#include <assert.h>
#include "Buffer_gl.h"

namespace Magnet
{
namespace HALgfx
{

void GLBuffer::GetDesc(BufferDesc& bufferDesc) const
{
	bufferDesc = mDesc;
}

void GLBuffer::Create(const BufferDesc& bufferDesc, const SubResourceData& pSubResourceData)
{
	glGenBuffers(1, &m_buffer);

	GLenum target = GetGLBindFlags(bufferDesc.bindFlags);

	glBindBuffer(target, m_buffer);
	if (glGetError() != GL_NO_ERROR)
	{
		assert(0);
	}

	glBufferData(target, bufferDesc.byteSize, pSubResourceData.pMem, GetGLUsage(bufferDesc.usage));

	if (glGetError() != GL_NO_ERROR)
	{
		assert(0);
	}
	
	glBindBuffer(target, 0);

}

} // namespace HALgfx
} // namespace Magnet