#include "Inputlayout_gl.h"

namespace Magnet
{
namespace HALgfx
{
	GLInputLayout::GLInputLayout(int iNumElements, const InputElementDesc *pInputElementDescs)
	{
		m_iNumElements = iNumElements;

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

	GLInputLayout::~GLInputLayout()
	{

	}


}
}