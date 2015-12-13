#ifndef FORMAT_PLAT_H
#define FORMAT_PLAT_H

#include <d3d11.h>
#include "Format.h"

namespace Magnet
{
namespace HALgfx
{
	DXGI_FORMAT GetD3DFormat(Format);
//	Format      GetHALFormat(DXGI_FORMAT);
} // namespace HALgfx
} // namespace Magnet

#endif