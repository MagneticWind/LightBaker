#ifndef IDEPTH_STENCIL_VIEW_H
#define IDEPTH_STENCIL_VIEW_H

#include "Format.h"

namespace Magnet
{

namespace HALgfx
{

enum DSVDimension
{
	DSV_DIMENSION_UNKNOWN,
	DSV_DIMENSION_TEXTURE2D
};

struct TEX2D_DSV
{
	unsigned int mipSlice;
};

struct DepthStencilViewDesc
{
	DepthStencilViewDesc()
	{
		flags = 0;
	}
	Format format;
	DSVDimension viewDimension;
	unsigned int flags;
	union
	{
		TEX2D_DSV texture2d;
	};
};

enum DSVflag
{
	DSV_READ_ONLY_DEPTH = 1u,
	DSV_READ_ONLY_STENCIL = (1u << 1)
};

class IDepthStencilView
{
public:
	virtual ~IDepthStencilView(){}
};

} // namespace HALgfx
} // namespace Magnet

#endif