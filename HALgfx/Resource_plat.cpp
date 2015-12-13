#include "Resource_plat.h"

namespace Magnet
{
namespace HALgfx
{

D3D11_USAGE GetD3DUsage(Usage usage)
{
	switch (usage)
	{
	case USAGE_IMMUTABLE:
		return D3D11_USAGE_IMMUTABLE;
	case USAGE_DYNAMIC:
		return D3D11_USAGE_DYNAMIC;
	case USAGE_STAGING:
		return D3D11_USAGE_STAGING;
	default:
		return D3D11_USAGE_DEFAULT;
	}
}

UINT GetD3DBindFlags(unsigned int bindFlags)
{
	UINT ret = 0;
	if (bindFlags & BIND_VERTEX_BUFFER)
	{
		ret |= D3D11_BIND_VERTEX_BUFFER;
	}
	if (bindFlags & BIND_INDEX_BUFFER)
	{
		ret |= D3D11_BIND_INDEX_BUFFER;
	}
	if (bindFlags & BIND_CONSTANT_BUFFER)
	{
		ret |= D3D11_BIND_CONSTANT_BUFFER;
	}
	if (bindFlags & BIND_SHADER_RESOURCE)
	{
		ret |= D3D11_BIND_SHADER_RESOURCE;
	}
	if (bindFlags & BIND_STREAM_OUTPUT)
	{
		ret |= D3D11_BIND_STREAM_OUTPUT;
	}
	if (bindFlags & BIND_RENDER_TARGET)
	{
		ret |= D3D11_BIND_RENDER_TARGET;
	}
	if (bindFlags & BIND_DEPTH_STENCIL)
	{
		ret |= D3D11_BIND_DEPTH_STENCIL;
	}
	if (bindFlags & BIND_UNORDERED_ACCESS)
	{
		ret |= D3D11_BIND_UNORDERED_ACCESS;
	}
	//if (miscFlags & MISC_FLAG_TEXTURE_GENERATE_MIPS)
	//{
	//	ret |= D3D11_BIND_RENDER_TARGET;
	//	ret |= D3D11_BIND_SHADER_RESOURCE;
	//}
	//if (bindFlags & BIND_RESOLVE_TARGET)
	//{
	//	if ((bindFlags & BIND_FLAG_DEPTH_STENCIL) == 0)
	//	{
	//		ret |= D3D11_BIND_RENDER_TARGET;
	//	}
	//}
	return ret;
}

UINT GetD3DAccessFlags(unsigned int flags)
{
	UINT ret = 0;
	if (flags & CPU_ACCESS_READ)
	{
		ret |= D3D11_CPU_ACCESS_READ;
	}
	if (flags & CPU_ACCESS_WRITE)
	{
		ret |= D3D11_CPU_ACCESS_WRITE;
	}
	return ret;
}

UINT GetD3DMiscFlags(unsigned int flags)
{
	UINT ret = 0;
	if (flags & MISC_TEXTURECUBE)
	{
		ret |= D3D11_RESOURCE_MISC_TEXTURECUBE;
	}
	if (flags & MISC_GENERATE_MIPS)
	{
		ret |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
	}
	if (flags & MISC_BUFFER_STRUCTURED)
	{
		ret |= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	}
	if (flags & MISC_DRAWINDIRECT_ARGS)
	{
		ret |= D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
	}
	return ret;
}
} // namespace HALgfx
} // namespace Magnet