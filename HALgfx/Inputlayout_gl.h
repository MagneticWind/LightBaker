#ifndef GL_INPUT_LAYOUT_H
#define GL_INPUT_LAYOUT_H

#include <glew.h>
#include "IInputLayout.h"


namespace Magnet
{
namespace HALgfx
{

class GLInputLayout : public IInputLayout
{
public:
	GLInputLayout(int iNumElements, const InputElementDesc *pInputElementDescs, int iStride);
	~GLInputLayout();

	int GetElementCount() const;
	const InputElementDesc& GetElementDesc(int iIndex) const;
	int GetStride() const;

	void CreateVAO(GLuint vertexBuffers);
	GLuint GetVAO() const;

private:
	int m_iNumElements;
	int m_iStride;
	InputElementDesc m_elementDesc[16]; // the max number of elements

	GLuint m_vao;
};

} // namespace HALgfx
} // namespace Magnet

#endif