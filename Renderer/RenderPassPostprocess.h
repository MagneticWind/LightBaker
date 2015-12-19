#ifndef RENDER_PASS_POSTPROCESS_H
#define RENDER_PASS_POSTPROCESS_H

#include "IRenderPass.h"

namespace Magnet
{

namespace HALgfx
{
class IDevice;
class IDeviceContext;
class IBuffer;
class IInputLayout;
}

namespace Renderer
{

class ShaderNode;

class RenderPassPostprocess : public IRenderPass
{
public:
	RenderPassPostprocess();
	virtual ~RenderPassPostprocess();

public:
	virtual void SetRenderState(HALgfx::IDeviceContext* pDeviceContext, const HALgfx::ViewPort& viewPort,
		HALgfx::IRenderTargetView* pRTV, HALgfx::IDepthStencilView* pDSV, HALgfx::IRasterizerState* pRState, HALgfx::IDepthStencilState* pDSState);
	virtual void Render(HALgfx::IDevice* pDevice, HALgfx::IDeviceContext* pDeviceContext);
	virtual PassType GetType();
	virtual void ClearDrawNodes();
	virtual void Setup(HALgfx::IDevice* pDevice);
	virtual std::list<ShaderNode*>& GetShaderNodeList();

	void SetParams(int iDimensionX, int iDimensionY, float fIntensityLevel);
	void SetHDRSRV(HALgfx::IShaderResourceView* pHDRSRV);

private:
	void Initialize(HALgfx::IDevice* pDevice);

private:
	std::list<ShaderNode*> m_lShaderNodes;
	bool m_bInitialized;

	int m_iDimensionX;
	int m_iDimensionY;
	float m_fIntensityLevel;

	HALgfx::IBuffer* m_pQuadVertexBuffer;
	HALgfx::IBuffer* m_pQuadIndexBuffer;
	HALgfx::IInputLayout* m_pInputLayout;

	HALgfx::IShaderResourceView* m_pSRVHDR;
};

inline void RenderPassPostprocess::SetParams(int iDimensionX, int iDimensionY, float fIntensityLevel)
{
	m_iDimensionX = iDimensionX;
	m_iDimensionY = iDimensionY;
	m_fIntensityLevel = fIntensityLevel;
}

inline void RenderPassPostprocess::SetHDRSRV(HALgfx::IShaderResourceView* pHDRSRV)
{
	m_pSRVHDR = pHDRSRV;
}

} // namespace Renderer
} // namespace Magnet
#endif