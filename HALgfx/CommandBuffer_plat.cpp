#include "CommandBuffer_plat.h"

namespace Magnet
{
namespace HALgfx
{
void CommandBuffer::Create(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->FinishCommandList(0, &m_pd3dCommandList);
}

ID3D11CommandList* CommandBuffer::GetD3DPtr()
{
	return m_pd3dCommandList;
}

} // namespace HALgfx
} // namespace Magnet