#ifndef INPUT_LAYOUT_H
#define INPUT_LAYOUT_H

#include <d3d11.h>
#include "IInputLayout.h"

namespace Magnet
{
namespace HALgfx
{

class InputLayout : public IInputLayout
{
public:
	InputLayout(int iNumElements, const InputElementDesc *pInputElementDescs, int iShaderCodeSize, const void* pShaderCode);
	~InputLayout();
	void Create(ID3D11Device* pDevice);
	ID3D11InputLayout* GetD3DPtr();

private:
	int m_iNumElements;
	InputElementDesc m_elementDesc[16]; // the max number of elements
	int m_iShaderCodeSize;
	const void* m_pShaderCode;
	ID3D11InputLayout* m_pInputLayout;
};

inline ID3D11InputLayout* InputLayout::GetD3DPtr()
{
	return m_pInputLayout;
}

} // namespace HALgfx
} // namespace Magnet

#endif