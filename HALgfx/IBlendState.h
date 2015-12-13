#ifndef IBLEND_STATE_H
#define IBLEND_STATE_H

namespace Magnet
{
namespace HALgfx
{

enum Blend
{
	BLEND_ZERO              = 1,
	BLEND_ONE               = 2,
	BLEND_SRC_COLOR         = 3,
	BLEND_INV_SRC_COLOR     = 4,
	BLEND_SRC_ALPHA         = 5,
	BLEND_INV_SRC_ALPHA     = 6,
	BLEND_DEST_ALPHA        = 7,
	BLEND_INV_DEST_ALPHA    = 8,
	BLEND_DEST_COLOR        = 9,
	BLEND_INV_DEST_COLOR    = 10,
	BLEND_SRC_ALPHA_SAT     = 11,
	BLEND_BLEND_FACTOR      = 14,
	BLEND_INV_BLEND_FACTOR  = 15,
	BLEND_SRC1_COLOR        = 16,
	BLEND_INV_SRC1_COLOR    = 17,
	BLEND_SRC1_ALPHA        = 18,
	BLEND_INV_SRC1_ALPHA    = 19
};

enum BlendOp
{
	BLEND_OP_ADD = 1,
	BLEND_OP_SUBTRACT = 2,
	BLEND_OP_REV_SUBTRACT = 3,
	BLEND_OP_MIN = 4,
	BLEND_OP_MAX = 5
};

struct RenderTargetBlendDesc
{
	bool blendEnable;
	Blend srcBlend;
	Blend dstBlend;
	BlendOp blendOp;
	Blend scrBlendAlpha;
	Blend dstBlendAlpha;
	BlendOp BlendOpAlpha;
	unsigned char renderTargetWriteMask;

};

struct BlendDesc
{
	bool alphaToCoverageEnable;
	bool independentBlendEnable;
	RenderTargetBlendDesc renderTarget[8];
};

class IBlendState
{
public:
	virtual ~IBlendState(){}
};

} // namespace HALgfx
} // namespace Magnet
#endif