#ifndef IRASTERIZER_STATE
#define IRASTERIZER_STATE

namespace Magnet
{
namespace HALgfx
{

enum FillMode
{
	FILL_WIREFRAME = 2,
	FILL_SOLID = 3
};

enum CullMode
{
	CULL_NONE = 1,
	CULL_FRONT = 2,
	CULL_BACK = 3
};

struct RasterizerDesc
{
	FillMode fillMode;
	CullMode cullMode;
	bool frontCounterClockwise;
	int depthBias;
	float depthBiasClamp;
	float slopeScaleDepthBias;
	bool depthClipEnable;
	bool scissorEnable;
	bool multisampleEnable;
	bool antialiasedLineEnable;
};

class IRasterizerState
{
public:
	virtual ~IRasterizerState(){}
};

} // namespace HALgfx
} // namespace Magnet

#endif