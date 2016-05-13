#ifndef RENDER_PASS_OPAQUE
#define RENDER_PASS_OPAQUE

#include <list>
#include <map>
#include <string>

#include "CBufferDefs.h"
#include "IRenderPass.h"
#include "GPUResource.h"

#include "HALgfx\Viewport.h"


namespace Magnet
{
namespace Math
{
	struct Matrix4f;
}

namespace HALgfx
{
	class IDevice;
	class IDeviceContext;
	class IShaderResourceView;
}

namespace Renderer
{

class ShaderNode;

typedef void(*CallBackCopyShadowParameters) (Math::Matrix4f* mShadowView, Math::Matrix4f** mProjection, HALgfx::IShaderResourceView* pShadowmapSRV);

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
	virtual void Setup(HALgfx::IDevice* pDevice, int iWidth, int iHeight);

	void SetLightIntensityLevel(float fLightIntensityLevel);

	void SetShadowParameters(Math::Matrix4f* mShadowView, Math::Matrix4f* mProjection, HALgfx::IShaderResourceView** pShadowmapSRV, Math::Vector4f* pRange);


private:
	std::list<ShaderNode*> m_lShaderNodes;

	float m_fLightIntensityLevel;

	Math::Matrix4f m_mShadowView;
	Math::Matrix4f m_mProjection[MAX_CASCADE_COUNT];
	HALgfx::ViewPort m_viewPorts[MAX_CASCADE_COUNT];
	Math::Vector4f m_v4Range;
	HALgfx::IShaderResourceView* m_pShadowSRV;

};

inline void RenderPassOpaque::SetLightIntensityLevel(float fLightIntensityLevel)
{
	m_fLightIntensityLevel = fLightIntensityLevel;
}


} // namespace Renderer
} // namespace Magnet

#endif