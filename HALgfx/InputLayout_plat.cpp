#include "InputLayout_plat.h"
#include "Format_plat.h"

namespace Magnet
{
namespace HALgfx
{
InputLayout::InputLayout(int iNumElements, const InputElementDesc *pInputElementDescs, int iShaderCodeSize, const void* pShaderCode)
{
	m_iNumElements = iNumElements;
	for (int i = 0; i < iNumElements; ++i)
	{
		m_elementDesc[i] = pInputElementDescs[i];
	}
	m_iShaderCodeSize = iShaderCodeSize;
	m_pShaderCode = pShaderCode;
}

InputLayout::~InputLayout()
{
	if(m_pInputLayout)
	{
		m_pInputLayout->Release();
		m_pInputLayout = 0;
	}
}

void InputLayout::Create(ID3D11Device* pDevice)
{
	D3D11_INPUT_ELEMENT_DESC elements[16];
	for (int i = 0; i < m_iNumElements; ++i)
	{
		elements[i].SemanticName = m_elementDesc[i].semantic;
		elements[i].SemanticIndex = m_elementDesc[i].semanticIndex;
		elements[i].Format = GetD3DFormat(m_elementDesc[i].format);
		elements[i].InputSlot = m_elementDesc[i].inputSlot;
		elements[i].AlignedByteOffset = m_elementDesc[i].alignedByteOffset;
		elements[i].InstanceDataStepRate = m_elementDesc[i].instanceDataStepRate;
		if(m_elementDesc[i].inputClassification == INPUT_PER_VERTEX_DATA)
			elements[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		else if (m_elementDesc[i].inputClassification == INPUT_PER_INSTANCE_DATA)
			elements[i].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	}

	pDevice->CreateInputLayout(elements, m_iNumElements, m_pShaderCode, m_iShaderCodeSize, &m_pInputLayout);
}

} // namespace HALgfx
} // namespace Magnet