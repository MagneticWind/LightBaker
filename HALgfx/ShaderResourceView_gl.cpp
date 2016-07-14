#include "ShaderResourceView_gl.h"
#include "Texture2d_gl.h"

namespace Magnet
{
	namespace HALgfx
	{
		GLShaderResourceView::GLShaderResourceView()
		{

		}

		GLShaderResourceView::~GLShaderResourceView()
		{

		}

		void GLShaderResourceView::Create(const ShaderResourceViewDesc& srvDesc, IResource* pResource)
		{
			switch (srvDesc.viewDimension)
			{
			case HALgfx::SRV_DIMENSION_TEXTURE2D:
				{
					GLTexture2d* pTexture2d = static_cast<GLTexture2d*>(pResource);
					m_texture = pTexture2d->GetGLHandle();
					m_type = RESOURCE_TEXTURE2D;
					break;
				}
			case HALgfx::SRV_DIMENSION_TEXTURECUBE:
				{
					GLTexture2d* pTexture2d = static_cast<GLTexture2d*>(pResource);
					m_texture = pTexture2d->GetGLHandle();
					m_type = RESOURCE_TEXTURECUBE;
					break;
				}
			}
		}
	}
}