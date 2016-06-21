#ifndef IBUFFER_H
#define IBUFFER_H

#include "IResource.h"

namespace Magnet
{
namespace HALgfx
{

class IDevice;

struct BufferDesc
{
	BufferDesc()
	{
		usage = USAGE_DEFAULT;
		byteSize = 0;
		byteStride = 0;
		bindFlags = BIND_DEFAULT;
		cpuAccessFlags = CPU_ACCESS_DEFAULT;
		miscFlags = MISC_DEFAULT;
	}
	Usage usage;
	unsigned int byteSize;
	unsigned int byteStride;
	unsigned int bindFlags;
	unsigned int cpuAccessFlags;
	unsigned int miscFlags;

};

class IBuffer : public IResource
{
public:
	virtual ~IBuffer(){}
	virtual void GetDesc(BufferDesc&) const = 0;
	virtual ResourceType GetType() const = 0;
	//virtual void Create(IDevice* pDevice, BufferDesc& bufferDesc, SubResourceData* pSubResourceData) = 0;
};

} // namespace HALgfx
} // namespace Magnet
#endif