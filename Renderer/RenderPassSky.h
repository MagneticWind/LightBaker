#ifndef RENDER_PASS_SKY
#define RENDER_PASS_SKY

#include <list>
#include <map>
#include <string>

#include "IRenderPass.h"
#include "GPUResource.h"

namespace Magnet
{

namespace HALgfx
{
	class IDevice;
	class IDeviceContext;
}

namespace Renderer
{

class ShaderNode;

class RenderPassSky : public IRenderPass
{
public:
	RenderPassSky();
	virtual ~RenderPassSky();

public:
	virtual void SetRenderState(HALgfx::IDeviceContext* pDeviceContext, const HALgfx::ViewPort& viewPort,
		HALgfx::IRenderTargetView* pRTV, HALgfx::IDepthStencilView* pDSV, HALgfx::IRasterizerState* pRState, HALgfx::IDepthStencilState* pDSState);
	virtual void Render(HALgfx::IDevice* pDevice, HALgfx::IDeviceContext* pDeviceContext);
	virtual PassType GetType();
	virtual void ClearDrawNodes();
	virtual void Setup(HALgfx::IDevice* pDevice);
	virtual std::list<ShaderNode*>& GetShaderNodeList();

private:
	std::list<ShaderNode*> m_lShaderNodes;
};

} // namespace Renderer
} // namespace Magnet

#endif