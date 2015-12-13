#ifndef COMPARISON_FUNC_PLAT_H
#define COMPARISON_FUNC_PLAT_H

#include "ComparisonFunc.h"
#include <d3d11.h>
namespace Magnet
{
namespace HALgfx
{
D3D11_COMPARISON_FUNC GetD3DComparisonFunc(ComparisonFunc func);
} // namespace HALgfx
} // namespace Magnet
#endif