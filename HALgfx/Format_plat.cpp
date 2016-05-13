#include <assert.h>
#include "Format_plat.h"

namespace Magnet
{
namespace HALgfx
{
DXGI_FORMAT GetD3DFormat(Format format)
{
	switch(format)
	{
	case FORMAT_R32G32B32A32_FLOAT:
		return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case FORMAT_R32G32B32_FLOAT:
		return DXGI_FORMAT_R32G32B32_FLOAT;
	case FORMAT_R16G16B16A16_FLOAT:
		return DXGI_FORMAT_R16G16B16A16_FLOAT;
	case FORMAT_R8G8B8A8_UINT:
		return DXGI_FORMAT_R8G8B8A8_UINT;
	case FORMAT_R8G8B8A8_UNORM:
		return DXGI_FORMAT_R8G8B8A8_UNORM;
	case FORMAT_R32_TYPELESS:
		return DXGI_FORMAT_R32_TYPELESS;
	case FORMAT_R32G32_FLOAT:
		return DXGI_FORMAT_R32G32_FLOAT;
	case FORMAT_D32_FLOAT:
		return DXGI_FORMAT_D32_FLOAT;
	case FORMAT_R32_FLOAT:
		return DXGI_FORMAT_R32_FLOAT;
	case FORMAT_R32_UINT:
		return DXGI_FORMAT_R32_UINT;
	case FORMAT_UNKNOWN:
		return DXGI_FORMAT_UNKNOWN;
	case FORMAT_D32_FLOAT_S8X24_UINT:
		return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	case FORMAT_R24G8_TYPELESS:
		return DXGI_FORMAT_R24G8_TYPELESS;
	case FORMAT_D24_UNORM_S8_UINT:
		return DXGI_FORMAT_D24_UNORM_S8_UINT;
	case FORMAT_R24_UNORM_X8_TYPELESS:
		return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	default:
		{
			assert(0);
		}
	};
}

//Format GetHALFormat(DXGI_FORMAT dxgi_format)
//{
//}

} // namespace HALgfx
} // namespace Magnet