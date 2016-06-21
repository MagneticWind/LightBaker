#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <Windows.h>	//TODO: move this to RenderWindow, so this lib can be platform independents

#include "IRenderSystem.h"
#include "IRenderTargetView.h"
#include "IShaderResourceView.h"

#include "Viewport.h"

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

class GLRenderSystem : public IRenderSystem
{

private:
	GLRenderSystem();
	~GLRenderSystem();
	GLRenderSystem(const GLRenderSystem&);
	GLRenderSystem& operator=(const GLRenderSystem&);

	static GLRenderSystem* ms_pInstance;

public:
	static void Initialize();
	static GLRenderSystem* GetInstance();
	static bool Exist();
	static void Terminate();

	virtual bool InitializeSystem(unsigned int uWidth, unsigned int uHeight, void* pWindowHandle) override;
	virtual void TerminateSystem() override;
	virtual void Present() override;

private:

	HDC m_hDC;

};

} // namespace HALgfx
} // namespace Magnet

#endif