#ifndef MACROS_H
#define MACROS_H
#include <math.h>

namespace Magnet
{
namespace Math
{
#define M_PI 3.1415926f

#define FLOAT_COMPARE_EPSILON 0.0000001f

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

} // namespace Math
} // namespace Magnet

#endif