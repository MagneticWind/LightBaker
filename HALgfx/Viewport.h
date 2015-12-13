#ifndef VIEWPORT_H
#define VIEWPORT_H

namespace Magnet
{
namespace HALgfx
{
struct ViewPort
{
	float topLeftX;
	float topLeftY;
	float width;
	float height;
	float minDepth;
	float maxDepth;
};
} // HALgfx
} // namespace Magnet

#endif