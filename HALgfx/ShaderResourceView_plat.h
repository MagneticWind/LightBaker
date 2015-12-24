#ifndef SHADER_RESOURCE_VIEW_H
#define SHADER_RESOURCE_VIEW_H

#include "IShaderResourceView.h"
#include <d3d11.h>

namespace Magnet
{
namespace HALgfx
{
class IResource;

class ShaderResourceView : public IShaderResourceView
{
public:
	ShaderResourceView();
	ShaderResourceView(ID3D11ShaderResourceView* pSRV);
	~ShaderResourceView();

	void Create(const ShaderResourceViewDesc& srvDesc, IResource* pResource, ID3D11Device* pDevice);

	ID3D11ShaderResourceView* GetD3DPtr();

private:
	ID3D11ShaderResourceView* m_pSRV;
};

} // namespace HALgfx
} // namespace Magnet

#endif