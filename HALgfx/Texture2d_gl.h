#ifndef GL_TEXTUER2D_H
#define GL_TEXTURE2D_H

#include "ITexture2d.h"

namespace Magnet
{
namespace HALgfx
{
class GLTexture2d : public ITexture2d
{
public:
	GLTexture2d();
	GLTexture2d(unsigned int uTexture);
	~GLTexture2d();

	void Create(const Texture2dDesc& texture2dDesc, const SubResourceData& subResourceData);

	unsigned int GetGLHandle();
	virtual ResourceType GetType() const;

private:
	unsigned int m_uTexture;
};

inline unsigned int GLTexture2d::GetGLHandle()
{
	return m_uTexture;
}

inline ResourceType GLTexture2d::GetType() const
{
	return RESOURCE_TEXTURE2D;
}
} // namespace HALgfx
} // namespace Magnet

#endif