#include <assert.h>
#include "Inputlayout_gl.h"

namespace Magnet
{
namespace HALgfx
{
	GLInputLayout::GLInputLayout(int iNumElements, const InputElementDesc *pInputElementDescs, int iStride)
	{
		m_iNumElements = iNumElements;
		m_iStride = iStride;
		m_vao = 0;

		for (int i = 0; i < m_iNumElements; ++i)
		{
			m_elementDesc[i] = pInputElementDescs[i];
		}

	}

	const InputElementDesc& GLInputLayout::GetElementDesc(int iIndex) const
	{
		return m_elementDesc[iIndex];
	}

	int GLInputLayout::GetElementCount() const
	{
		return m_iNumElements;
	}

	int GLInputLayout::GetStride() const
	{
		return m_iStride;
	}

	GLInputLayout::~GLInputLayout()
	{

	}

	void GLInputLayout::CreateVAO(GLuint vertexBuffer)
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		//glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBindVertexBuffer(0, vertexBuffer, 0, m_iStride);

		for (int i = 0; i < m_iNumElements; ++i)
		{
			const InputElementDesc& elementDesc = m_elementDesc[i];
			
			// assume attribLocation follow the order of vertex buffe
			//GLint attribLocation = glGetAttribLocation(m_currentProgram, elementDesc.semantic);

			glEnableVertexAttribArray(i);

			GLintptr offset = elementDesc.alignedByteOffset;
			//glVertexAttribPointer(i, elementDesc.size, GL_FLOAT, GL_FALSE, m_iStride, (GLvoid*)offset);
			glVertexAttribFormat(i, elementDesc.size, GL_FLOAT, GL_FALSE, elementDesc.alignedByteOffset);
			glVertexAttribBinding(i, 0);

			GLenum err = glGetError();
			if (err != GL_NO_ERROR)
			{
				assert(0);
			}
		}

		glBindVertexArray(0);
	}

	GLuint GLInputLayout::GetVAO() const
	{
		return m_vao;
	}

}
}