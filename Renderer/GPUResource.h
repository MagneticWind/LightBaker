#ifndef GPU_RESOURCE_H
#define GPU_RESOURCE_H

#include "HALgfx\IInputLayout.h"

namespace Magnet
{
namespace HALgfx
{
class IBuffer;
class ITexture2d;
class IShaderResourceView;
struct InputElementDesc;
}

namespace Renderer
{
	struct MeshResource
	{
		HALgfx::InputElementDesc m_aInputElementsDesc[MAX_NUM_ELEMENTS];
		int m_iInputelementCount;

		int m_iPrimitivesCount;
		HALgfx::IInputLayout* m_pInputLayout;
		HALgfx::IBuffer* m_pVertexBuffer;
		HALgfx::IBuffer* m_pIndexBuffer;
	};

	struct TextureResource
	{
		HALgfx::ITexture2d* m_pTexture2D;
		HALgfx::IShaderResourceView* m_pShaderResourceView;
	};

} // namespace Magnet
} // namespace Renderer

#endif