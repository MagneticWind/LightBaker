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

	if (pSubResourceData.pMem)
	{
		GLenum bufferType = GetGLBindFlags(bufferDesc.bindFlags);

		glBindBuffer(bufferType, m_buffer);
		glBufferData(bufferType, sizeof(bufferDesc.byteSize), pSubResourceData.pMem, GetGLUsage(bufferDesc.usage));
		glBindBuffer(bufferType, 0);
	}
	else
	{
		GLenum bufferType = GetGLBindFlags(bufferDesc.bindFlags);

		glBindBuffer(bufferType, m_buffer);
		glBindBuffer(bufferType, 0);
	}

}

} // namespace HALgfx
} // namespace Magnet