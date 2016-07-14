#ifndef GPU_RESOURCE_H
#define GPU_RESOURCE_H

#include <string.h>

#include "HALgfx\IInputLayout.h"

namespace Magnet
{
namespace HALgfx
{
class IBuffer;
class ITexture2d;
class IResource;
class IShaderResourceView;
class ISamplerState;
}

namespace Renderer
{
	struct MeshResource
	{
		MeshResource() : m_iNumElements(0), m_iPrimitivesCount(0), m_iStride(0), m_pVertexBuffer(NULL), m_pIndexBuffer(NULL)
		{}

		void GetSemanticString(HALgfx::InputSemantic eSemantic, char* pOut)
		{
			switch (eSemantic)
			{
			case HALgfx::INPUT_SEMANTIC_POSITION:
				strcpy(pOut, "POSITION");
				return;
			case HALgfx::INPUT_SEMANTIC_NORMAL:
				strcpy(pOut, "NORMAL");
				return;
			case HALgfx::INPUT_SEMANTIC_TEXCOORD:
				strcpy(pOut, "TEXCOORD");
				return;
			case HALgfx::INPUT_SEMANTIC_COLOR:
				strcpy(pOut, "COLOR");
				return;
			case HALgfx::INPUT_SEMANTIC_TANGENT:
				strcpy(pOut, "TANGENT");
				return;
			case HALgfx::INPUT_SEMANTIC_BINORMAL:
				strcpy(pOut, "BINORMAL");
				return;
			}
		}

		void AddElement(HALgfx::InputSemantic semantic, int iNumFloats)
		{
			GetSemanticString(semantic, m_aInputElementsDesc[m_iNumElements].semantic);

			m_caInputLayoutString[m_iNumElements] = m_aInputElementsDesc[m_iNumElements].semantic[0];
			m_caInputLayoutString[m_iNumElements + 1] = '\0';

			if (iNumFloats == 3)
				m_aInputElementsDesc[m_iNumElements].format = HALgfx::FORMAT_R32G32B32_FLOAT;
			else if (iNumFloats == 2)
				m_aInputElementsDesc[m_iNumElements].format = HALgfx::FORMAT_R32G32_FLOAT;
			else if (iNumFloats == 4)
				m_aInputElementsDesc[m_iNumElements].format = HALgfx::FORMAT_R32G32B32A32_FLOAT;

			m_aInputElementsDesc[m_iNumElements].size = iNumFloats;
			m_aInputElementsDesc[m_iNumElements].inputSlot = 0;
			m_aInputElementsDesc[m_iNumElements].alignedByteOffset = m_iStride;
			m_aInputElementsDesc[m_iNumElements].semanticIndex = 0;
			m_aInputElementsDesc[m_iNumElements].instanceDataStepRate = 0;
			m_aInputElementsDesc[m_iNumElements].inputClassification = HALgfx::INPUT_PER_VERTEX_DATA;

			m_iStride += iNumFloats * sizeof(float);
			m_iNumElements++;
		}

		int m_iNumElements;
		int m_iPrimitivesCount;
		int m_iStride;

		char m_caInputLayoutString[64];

		HALgfx::IBuffer* m_pVertexBuffer;
		HALgfx::IBuffer* m_pIndexBuffer;

		HALgfx::InputElementDesc m_aInputElementsDesc[MAX_NUM_ELEMENTS];
	};

	struct TextureResource
	{
		TextureResource() : m_pTexture(NULL), m_pShaderResourceView(NULL), m_pSampler(NULL)
		{
			m_caLabel[0] = '\0';
		}

		char m_caLabel[256];
		HALgfx::IResource* m_pTexture;
		HALgfx::IShaderResourceView* m_pShaderResourceView;
		HALgfx::ISamplerState* m_pSampler;
	};

} // namespace Magnet
} // namespace Renderer

#endif