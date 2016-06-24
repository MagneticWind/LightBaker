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
			m_type = pResource->GetType();

			switch (m_type)
			{
			case RESOURCE_TEXTURE2D:
				{
					GLTexture2d* pTexture2d = static_cast<GLTexture2d*>(pResource);
					m_texture = pTexture2d->GetGLHandle();
				}
				break;
			}
		}
	}
}