#ifndef IRENDER_TARGET_VIEW
#define IRENDER_TARGET_VIEW

#include "Format.h"

namespace Magnet
{
namespace HALgfx
{

#define MAX_NUM_RTVS 8

enum RTVDimension
{
	RTV_DIMENSION_UNKNOWN = 0,
	RTV_DIMENSION_BUFFER = 1,
	RTV_DIMENSION_TEXTURE2D = 2
};

struct TEX2D_RTV
{
	unsigned int mipSlice;
};

struct RenderTargetViewDesc
{
	Format format;
	RTVDimension viewDimension;
	union
	{
		TEX2D_RTV texture2d;
	};
};

class IRenderTargetView
{
public:
	virtual ~IRenderTargetView(){}
};
} // namespace HALgfx
} // namespace Magnet
#endif