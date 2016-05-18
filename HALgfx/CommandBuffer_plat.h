#ifndef COMMAND_BUFFER_H
#define COMMAND_BUFFER_H

#include <D3D11.h>
#include "ICommandBuffer.h"

namespace Magnet
{
namespace HALgfx
{
class CommandBuffer : public ICommandBuffer
{
public:
	void Create(ID3D11DeviceContext* pDeviceContext);
	ID3D11CommandList* GetD3DPtr();
private:
	ID3D11CommandList* m_pd3dCommandList;
};
} // namespace HALgfx
} // namespace Magnet

#endif