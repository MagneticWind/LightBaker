#ifndef RENDER_PASS_OPAQUE
#define RENDER_PASS_OPAQUE

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

class RenderPassOpaque : public IRenderPass
{
public:
	RenderPassOpaque();
	virtual ~RenderPassOpaque();

public:
	virtual void SetRenderState(HALgfx::IDeviceContext* pDeviceContext, const HALgfx::ViewPort& viewPort,
		HALgfx::IRenderTargetView* pRTV, HALgfx::IDepthStencilView* pDSV, HALgfx::IRasterizerState* pRState, HALgfx::IDepthStencilState* pDSState);
	virtual void Render(HALgfx::IDevice* pDevice, HALgfx::IDeviceContext* pDeviceContext);
	virtual PassType GetType();
	virtual void ClearDrawNodes();
	virtual void Setup(HALgfx::IDevice* pDevice);

	void SetLightIntensityLevel(float fLightIntensityLevel);

private:
	std::list<ShaderNode*> m_lShaderNodes;

	float m_fLightIntensityLevel;

};

inline void RenderPassOpaque::SetLightIntensityLevel(float fLightIntensityLevel)
{
	m_fLightIntensityLevel = fLightIntensityLevel;
}


} // namespace Renderer
} // namespace Magnet

#endif