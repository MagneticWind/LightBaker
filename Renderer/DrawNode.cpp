#include <stdlib.h>

#include "DrawNode.h"

namespace Magnet
{
namespace Renderer
{

DrawNode::DrawNode() : m_iNumberOfSamplers(0), m_iNumberOfSRVs(0), m_pVertexBuffer(0), m_pIndexBuffer(0), m_iNumberOfVSConstBuffers(0), m_iNumberOfPSConstBuffers(0)
{
	for (int i = 0; i < MAX_NUMBER_SRVS; ++i)
	{
		m_ppSRVs[i] = 0;
	}

	for (int i = 0; i < MAX_NUMBER_BUFFERS; ++i)
	{
		m_pVSCBufferData[i] = 0;
		m_pPSCBufferData[i] = 0;
	}
}

DrawNode::~DrawNode()
{
	for (int i = 0; i < MAX_NUMBER_SRVS; ++i)
	{
		m_ppSRVs[i] = 0;
	}

	for (int i = 0; i < MAX_NUMBER_BUFFERS; ++i)
	{
		m_pVSCBufferData[i] = 0;
		m_pPSCBufferData[i] = 0;
	}

	m_pVertexBuffer = 0;
	m_pIndexBuffer = 0;
}

void DrawNode::AddSRV(HALgfx::IShaderResourceView* pSRV)
{
	m_ppSRVs[m_iNumberOfSRVs++] = pSRV;
}

void* DrawNode::CreateCBufferData(int iSize, HALgfx::ShaderType eType)
{
	void* pBuffer = 0;

	if (eType == HALgfx::VERTEX_SHADER)
	{
		pBuffer = malloc(iSize);
		m_pVSCBufferData[m_iNumberOfVSConstBuffers++] = pBuffer;
	}
	else if (eType == HALgfx::PIXEL_SHADER)
	{
		pBuffer = malloc(iSize);
		m_pPSCBufferData[m_iNumberOfPSConstBuffers++] = pBuffer;
	}

	return pBuffer;
}

void DrawNode::DestroyCBufferData(void* pBuffer)
{
	free(pBuffer);
}

} // namespace Renderer
} // namespace Magnet