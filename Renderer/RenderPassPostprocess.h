#ifndef RENDER_PASS_POSTPROCESS_H
#define RENDER_PASS_POSTPROCESS_H
#include "CBufferDefs.h"
#include "IRenderPass.h"

namespace Magnet
{

namespace HALgfx
{
class IDevice;
class IDeviceContext;
class IBuffer;
class IInputLayout;
class IShader;
class IUnorderedAccessView;
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

	void SetParams(int iDimensionX, int iDimensionY, float fIntensityLevel);
	void SetHDRSRV(HALgfx::IShaderResourceView* pHDRSRV);
	void SetDepthSRV(HALgfx::IShaderResourceView* pDepthSRV);

private:
	void Initialize(HALgfx::IDevice* pDevice);
	void GenerateSSAOSamples();
	void ComputeAvgLuminance();

private:
	ShaderNode* m_pSSAOShaderNode;
	ShaderNode* m_pTonemappingShaderNode;

	bool m_bInitialized;

	int m_iDimensionX;
	int m_iDimensionY;
	float m_fIntensityLevel;

	HALgfx::IBuffer* m_pQuadVertexBuffer;
	HALgfx::IBuffer* m_pQuadIndexBuffer;
	HALgfx::IInputLayout* m_pInputLayout;

	HALgfx::IShaderResourceView* m_pSRVHDR;
	HALgfx::IShaderResourceView* m_pSRVDepth;

	HALgfx::ITexture2d* m_pSSAOTexture;
	HALgfx::IShaderResourceView* m_pSSAOTextureSRV;
	HALgfx::IRenderTargetView* m_pSSAORTV;
	CBufferSSAO m_ssaoParams;

	HALgfx::IRenderTargetView* m_pFinalRTV;
	HALgfx::IDepthStencilView* m_pDSV;

	ShaderNode* m_pReduceTo1DComputeShader;
	ShaderNode* m_pReduceTo1PixelComputeShader;

	HALgfx::IBuffer* m_pReductionBuffer0;
	HALgfx::IBuffer* m_pReductionBuffer1;
	HALgfx::IShaderResourceView* m_pReductionSRV0;
	HALgfx::IShaderResourceView* m_pReductionSRV1;
	HALgfx::IUnorderedAccessView* m_pReductionUAV0;
	HALgfx::IUnorderedAccessView* m_pReductionUAV1;
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

inline void RenderPassPostprocess::SetDepthSRV(HALgfx::IShaderResourceView* pDepthSRV)
{
	m_pSRVDepth = pDepthSRV;
}

} // namespace Renderer
} // namespace Magnet
#endif