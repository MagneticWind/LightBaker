#ifndef RENDER_TARGET_VIEW_H
#define RENDER_TARGET_VIEW_H

#include<d3d11.h>
#include "IRenderTargetView.h"


namespace Magnet
{
namespace HALgfx
{
class IResource;

class RenderTargetView : public IRenderTargetView
{
public:
	RenderTargetView();
	~RenderTargetView();

	void Create(const RenderTargetViewDesc& desc, IResource* pResource, ID3D11Device* pd3dDevice);
	void Create(IDXGISwapChain* swapChain, ID3D11Device* pd3dDevice);

	ID3D11RenderTargetView* GetD3DPtr();

private:
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pRTV;
	//ID3D11Resource* m_pResource;
};
} // namespace HALgfx
} // namespace Magnet
#endif