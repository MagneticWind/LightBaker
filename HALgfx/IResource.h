#ifndef IRESOURCE_H
#define IRESOURCE_H

namespace Magnet
{
namespace HALgfx
{

enum Usage
{
	USAGE_DEFAULT,      // GPU read\write
	USAGE_DYNAMIC,      // CPU write, GPU read
	USAGE_IMMUTABLE,    // GPU read
	USAGE_STAGING       // CPU read\write, GPU read\write. Restricted to copy operations.
};

enum BindFlag
{
	BIND_DEFAULT = 0,
	BIND_VERTEX_BUFFER    = (1u << 0),
	BIND_INDEX_BUFFER     = (1u << 1),
	BIND_CONSTANT_BUFFER  = (1u << 2),
	BIND_SHADER_RESOURCE  = (1u << 3),
	BIND_STREAM_OUTPUT    = (1u << 4),
	BIND_RENDER_TARGET    = (1u << 5),
	BIND_DEPTH_STENCIL    = (1u << 6),
	BIND_UNORDERED_ACCESS = (1u << 7)
};

enum CPUAccessFlag
{
	CPU_ACCESS_DEFAULT = 0,
	CPU_ACCESS_WRITE  = 0x10000L,
	CPU_ACCESS_READ   = 0x20000L
};

enum MapType
{
	MAP_READ,
	MAP_WRITE,
	MAP_READ_WRITE,
	MAP_WRITE_DISCARD,
	MAP_WRITE_NO_OVERWRITE
};

enum MiscFlag
{
	MISC_DEFAULT       = 0,
	MISC_GENERATE_MIPS = (1u << 0),
	MISC_SHARED        = (1u << 1),
	MISC_TEXTURECUBE   = (1u << 2),
	MISC_DRAWINDIRECT_ARGS = (1u << 3),
	MISC_BUFFER_STRUCTURED = (1u << 4)

};

struct SubResourceData
{
	SubResourceData() : pMem(0), uMemPitch(0), uMemSlicePitch(0)
	{
	}
	void* pMem;
	unsigned int uMemPitch;
	unsigned int uMemSlicePitch;
};

struct SampleDesc
{
	SampleDesc() : uCount(1), uQuality(0)
	{
	}
	unsigned int uCount;
	unsigned int uQuality;
};

enum ResourceType
{
	RESOURCE_BUFFER,
	RESOURCE_TEXTURE1D,
	RESOURCE_TEXTURE2D

};

class IResource
{
public:
	virtual ~IResource() {}
	virtual ResourceType GetType() const = 0;
};

} // namespace HALgfx
} // namespace Magnet

#endif