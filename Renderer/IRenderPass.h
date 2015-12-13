#ifndef IRENDER_PASS_H
#define IRENDER_PASS_H

#include <list>

namespace Magnet
{

namespace Math
{
class Frustum;
}

namespace HALgfx
{
class IDevice;
class IDeviceContext;
class IRenderTargetView;
class IDepthStencilView;
class IRasterizerState;
class IDepthStencilState;
struct ViewPort;
}

namespace Renderer
{
class IRenderObject;
class ShaderNode;

enum PassType
{
	PASS_OPAQUE,
	PASS_NUMBER
};

class IRenderPass
{
public:
	IRenderPass(){}
	virtual ~IRenderPass(){}
public:
	virtual void SetRenderState(HALgfx::IDeviceContext* pDeviceContext, const HALgfx::ViewPort& viewPort,
		HALgfx::IRenderTargetView* pRTV, HALgfx::IDepthStencilView* pDSV, HALgfx::IRasterizerState* pRState, HALgfx::IDepthStencilState* pDSState) = 0;
	virtual void Render(HALgfx::IDevice* pDevice, HALgfx::IDeviceContext* pDeviceContext) = 0;
	virtual PassType GetType() = 0;
	virtual void ClearDrawNodes() = 0;
	virtual void Setup(HALgfx::IDevice* pDevice) = 0;
	virtual std::list<ShaderNode*>& GetShaderNodeList() = 0;
};
} // namespace Renderer
} // namespace Magnet

#endif