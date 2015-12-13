#ifndef RESOURCE_H
#define RESOURCE_H

#include <d3d11.h>
#include "IResource.h"

namespace Magnet
{
namespace HALgfx
{
D3D11_USAGE GetD3DUsage(Usage usage);
UINT GetD3DBindFlags(unsigned int bindFlags);
UINT GetD3DAccessFlags(unsigned int flags);
UINT GetD3DMiscFlags(unsigned int flags);

} // namespace HALgfx
} // namespace Magnet

#endif