#ifndef COMPARISON_FUNC
#define COMPARISON_FUNC

namespace Magnet
{
namespace HALgfx
{
enum ComparisonFunc
{
	COMPARISON_NEVER = 1,
	COMPARISON_LESS  = 2,
	COMPARISON_EQUAL = 3,
	COMPARISON_LESS_EQUAL = 4,
	COMPARISON_GREATER    = 5,
	COMPARISON_NOT_EQUAL  = 6,
	COMPARISON_GREATER_EQUAL = 7,
	COMPARISON_ALWAYS        = 8
};
} // HALgfx
} // namespace Magnet
#endif