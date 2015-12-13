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

	};
}

//Format GetHALFormat(DXGI_FORMAT dxgi_format)
//{
//}

} // namespace HALgfx
} // namespace Magnet