#include "Buffer_gl.h"

namespace Magnet
{
namespace HALgfx
{
void GLBuffer::Create(BufferDesc& bufferDesc, SubResourceData* pSubResourceData)
{
	glGenBuffers(1, &m_buffer);

	if (pSubResourceData->pMem)
	{
		GLenum bufferType = GetGLBindFlags(bufferDesc.bindFlags);

		glBindBuffer(bufferType, m_buffer);

		glBufferData(bufferType, sizeof(bufferDesc.byteSize), pSubResourceData->pMem, GetGLUsage(bufferDesc.usage));

				
	}
	else
	{
	}

}

} // namespace HALgfx
} // namespace Magnet