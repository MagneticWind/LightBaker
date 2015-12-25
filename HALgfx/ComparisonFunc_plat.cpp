#include <assert.h>
#include "ComparisonFunc_plat.h"

namespace Magnet
{
namespace HALgfx
{

//------------------------------------------------------------------
D3D11_COMPARISON_FUNC GetD3DComparisonFunc(ComparisonFunc func)
{
	switch(func)
	{
	case COMPARISON_NEVER:
		return D3D11_COMPARISON_NEVER;
	case COMPARISON_LESS:
		return D3D11_COMPARISON_LESS;
	case COMPARISON_EQUAL:
		return D3D11_COMPARISON_EQUAL;
	case COMPARISON_GREATER:
		return D3D11_COMPARISON_GREATER;
	case COMPARISON_NOT_EQUAL:
		return D3D11_COMPARISON_NOT_EQUAL;
	case COMPARISON_GREATER_EQUAL:
		return D3D11_COMPARISON_GREATER_EQUAL;
	case COMPARISON_ALWAYS:
		return D3D11_COMPARISON_ALWAYS;
	case COMPARISON_LESS_EQUAL:
		return D3D11_COMPARISON_LESS_EQUAL;
	default:
		assert(0);
	}
}
} // namespace HALgfx
} // namespace Magnet