#ifndef GL_SHADER_RESOURCE_VIEW_H
#define GL_SHADER_RESOURCE_VIEW_H

#include "glew.h"

#include "IShaderResourceView.h"

namespace Magnet
{
namespace HALgfx
{
class GLShaderResourceView : public IShaderResourceView
{
public:
	GLShaderResourceView();
	~GLShaderResourceView();

	void Create(const ShaderResourceViewDesc& srvDesc, IResource* pResource);
	ResourceType GetType() const;

	GLuint GetGLHandle() const;

private:
	GLuint m_texture;
	ResourceType m_type;
};

inline GLuint GLShaderResourceView::GetGLHandle() const
{
	return m_texture;
}

inline ResourceType GLShaderResourceView::GetType() const
{
	return m_type;
}

}
}

#endif