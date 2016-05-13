#ifndef RENDER_PASS_SHADOW
#define RENDER_PASS_SHADOW

#include <list>
#include <string>

#include "HALgfx\Viewport.h"

#include "IRenderPass.h"
#include "GPUResource.h"
#include "CBufferDefs.h"

#include "Math\Frustum.h"
#include "Math\Matrix4f.h"

namespace Magnet
{

namespace HALgfx
{
class IDevice;
class IDeviceContext;
class ITexture2D;
class IDepthStencilView;
class IShaderResourceView;
}

namespace Renderer
{

class ShaderNode;

class RenderPassShadow : public IRenderPass
{
public:
	RenderPassShadow();
	virtual ~RenderPassShadow();

public:
	virtual void SetRenderState(HALgfx::IDeviceContext* pDeviceContext, const HALgfx::ViewPort& viewPort,
		HALgfx::IRenderTargetView* pRTV, HALgfx::IDepthStencilView* pDSV, HALgfx::IRasterizerState* pRState, HALgfx::IDepthStencilState* pDSState);
	virtual void Render(HALgfx::IDevice* pDevice, HALgfx::IDeviceContext* pDeviceContext);
	virtual PassType GetType();
	virtual void ClearDrawNodes();
	virtual void Setup(HALgfx::IDevice* pDevice, int iWidth, int iHeight);

public:
	void Initialize(HALgfx::IDevice* pDevice, int iWidth, int iHeight);

	void GetShadowParameters(Math::Matrix4f* mShadowView, Math::Matrix4f* mProjection, HALgfx::IShaderResourceView** pShadowmapSRV, Math::Vector4f* pRange);

private:
	std::list<ShaderNode*> m_lShaderNodes;

	// shadowmap
	HALgfx::ITexture2d* m_pTexture2dShadowmap;
	HALgfx::IDepthStencilView* m_pShadowmapDSV;
	HALgfx::IShaderResourceView* m_pShadowmapSRV;

	Math::Frustum m_frustum;
	Math::Matrix4f m_mShadowView;
	Math::Matrix4f m_mProjection[MAX_CASCADE_COUNT];
	HALgfx::ViewPort m_viewPorts[MAX_CASCADE_COUNT];

	bool m_bInitialized;
};

} // namespace Renderer
} // namespace Magnet

#endif