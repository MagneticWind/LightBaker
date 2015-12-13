#ifndef IDEPTH_STENCIL_STATE
#define IDEPTH_STENCIL_STATE

#include "ComparisonFunc.h"

namespace Magnet
{
namespace HALgfx
{
enum DepthWriteMask
{
	DEPTH_WRITE_MASK_ZERO = 0,
	DEPTH_WRITE_MASK_ALL  = 1
};

enum StencilOP
{
	STENCIL_OP_KEEP = 1,
	STENCIL_OP_ZERO = 2,
	STENCIL_OP_REPLACE = 3,
	STENCIL_OP_INCR_SAT = 4,
	STENCIL_OP_DECR_SAT = 5,
	STENCIL_OP_INVERT   = 6,
	STENCIL_OP_INCR     = 7,
	STENCIL_OP_DECR     = 8
};

struct DepthStencilOPDesc
{
	StencilOP stencilFailOp;
	StencilOP stencilDepthFailOp;
	StencilOP stencilPassOp;
	ComparisonFunc stencilFunc;
};

struct DepthStencilDesc
{
	DepthStencilDesc()
	{
		depthEnable = true;
		stencilEnable = true;
		stencilReadMask = 0;
		stencilWriteMask = 0;

		depthFunc = COMPARISON_LESS;
	}
	bool depthEnable;
	DepthWriteMask depthWriteMask;
	unsigned int stencilReadMask;
	unsigned int stencilWriteMask;
	ComparisonFunc depthFunc;
	bool stencilEnable;
	DepthStencilOPDesc frontFace;
	DepthStencilOPDesc backFace;
};

class IDepthStencilState
{
public:
	virtual ~IDepthStencilState(){}
};

} // namespace HALgfx
} // namespace Magnet

#endif