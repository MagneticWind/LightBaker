#ifndef DEPTH_STENCIL_VIEW_H
#define DEPTH_STENCIL_VIEW_H

#include <d3d11.h>
#include "IDepthStencilView.h"

namespace Magnet
{

namespace HALgfx
{
class IResource;

class DepthStencilView : public IDepthStencilView
{
public:
	DepthStencilView();
	~DepthStencilView();
	void Create(const DepthStencilViewDesc& desc, IResource* pResource, ID3D11Device* pDevice);
	ID3D11DepthStencilView* GetD3DPtr();

private:
	ID3D11DepthStencilView* m_pDSV;
};
} // namespace HALgfx
} // namespace Magnet
#endif