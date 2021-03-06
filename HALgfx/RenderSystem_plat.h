#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <d3d11.h>
#include "Viewport.h"

#include "IRenderSystem.h"
#include "IRenderTargetView.h"
#include "IShaderResourceView.h"

//#define USE_COMMAND_BUFFER

namespace Magnet
{
namespace HALgfx
{

class Device;
class IDevice;
class IDeviceContext;
class IRenderTargetView;
class IDepthStencilView;
class IDepthStencilState;
class IBlendState;
class IShader;
class ITexture2d;
class IRasterizerState;
class ISamplerState;

class RenderSystem : public IRenderSystem
{
private:
	RenderSystem();
	~RenderSystem();
	RenderSystem(const RenderSystem&);
	RenderSystem& operator=(const RenderSystem&);

	static RenderSystem* ms_pInstance;

public:
	static void Initialize();
	static RenderSystem* GetInstance();
	static bool Exist();
	static void Terminate();

	virtual bool InitializeSystem(unsigned int uWidth, unsigned int uHeight, void* pWindowHandle) override;
	virtual void TerminateSystem() override;
	virtual void Present() override;

	void GetSHFromCubemap(float faSHRed[9], float faSHGreen[9], float faSHBlue[9]) const;

private:
	
	D3D_DRIVER_TYPE m_driverType;
	D3D_FEATURE_LEVEL m_featureLevel;
	IDXGISwapChain* m_pSwapChain;

	float m_faSHRed[9];
	float m_faSHGreen[9];
	float m_faSHBlue[9];
};

} // namespace HALgfx
} // namespace Magnet

#endif