#ifndef GL_INPUT_LAYOUT_H
#define GL_INPUT_LAYOUT_H

#include "IInputLayout.h"

namespace Magnet
{
namespace HALgfx
{

class GLInputLayout : public IInputLayout
{
public:
	GLInputLayout(int iNumElements, int iStride, const InputElementDesc *pInputElementDescs);
	~GLInputLayout();

	int GetElementCount() const;
	int GetStride() const;
	const InputElementDesc& GetElementDesc(int iIndex) const;

private:
	int m_iNumElements;
	int m_iStride;
	InputElementDesc m_elementDesc[16]; // the max number of elements
};

} // namespace HALgfx
} // namespace Magnet

#endif