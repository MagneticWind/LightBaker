#ifndef UNORDERED_ACCESS_VIEW_H
#define UNORDERED_ACCESS_VIEW_H

#include "IUnorderedAccessView.h"
#include <d3d11.h>

namespace Magnet
{
namespace HALgfx
{
class IResource;

class UnorderedAccessView : public IUnorderedAccessView
{
public:
	UnorderedAccessView();
	UnorderedAccessView(ID3D11UnorderedAccessView* pUAV);
	~UnorderedAccessView();

	void Create(const UnorderedAccessViewDesc& srvDesc, IResource* pResource, ID3D11Device* pDevice);

	ID3D11UnorderedAccessView* GetD3DPtr();

private:
	ID3D11UnorderedAccessView* m_pUAV;
};

} // namespace HALgfx
} // namespace Magnet

#endif