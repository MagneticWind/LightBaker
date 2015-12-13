#ifndef SURFACE_H
#define SURFACE_H

#include "Math\AABB.h"
#include "Math\Sphere.h"
#include "Math\Matrix4f.h"
#include "IGeometry.h"
#include "IMaterial.h"
#include "Texture.h"

namespace Magnet
{
namespace Scene
{

#define MAX_NUM_TEXTURES 8

class Surface
{
public:
	Surface()
	{
		m_iNumTextures = 0;
	}
	~Surface();

	const Math::AABB& GetBBox() const;
	const Math::Sphere& GetBSphere() const;
	const IMaterial* GetMaterial() const;
	const IGeometry* GetGeometry() const;

	const Texture* GetTexture(int iIndex) const;
	int GetNumTextures() const;

	void SetMaterial(IMaterial* pMaterial);
	void AddTexture(Texture* pTexture);
	void SetGeometry(IGeometry* pGeometry);

	//void* GetCBufferPtr(HALgfx::ShaderType eType, PassType ePassType, int iIndex)
	//{
	//	if(eType == HALgfx::VERTEX_SHADER)
	//		return m_ppVShaderCBufferPtr[ePassType][iIndex];
	//	else if (eType == HALgfx::PIXEL_SHADER)
	//		return m_ppPShaderCBufferPtr[ePassType][iIndex];
	//}

private:
	IGeometry* m_pGeometry;
	IMaterial* m_pMaterial;
	Texture* m_ppTextures[MAX_NUM_TEXTURES];
	int m_iNumTextures;

	// AABB and Sphere here is at object space or original world space when loading the model
	// Not considering transformations in world space when updating
	Math::AABB m_BBox;
	Math::Sphere m_BSphere;
};

//------------------------------------------------------------------
inline const Math::AABB& Surface::GetBBox() const
{
	return m_BBox;
}

//------------------------------------------------------------------
inline const Math::Sphere& Surface::GetBSphere() const
{
	return m_BSphere;
}

//------------------------------------------------------------------
inline const IMaterial* Surface::GetMaterial() const
{
	return m_pMaterial;
}

//------------------------------------------------------------------
inline const IGeometry* Surface::GetGeometry() const
{
	return m_pGeometry;
}

//------------------------------------------------------------------
inline void Surface::SetMaterial(IMaterial* pMaterial)
{
	m_pMaterial = pMaterial;
}

//------------------------------------------------------------------
inline void Surface::SetGeometry(IGeometry* pGeometry)
{
	m_pGeometry = pGeometry;
}

//------------------------------------------------------------------
inline void Surface::AddTexture(Texture* pTexture)
{
	m_ppTextures[m_iNumTextures] = pTexture;
	m_iNumTextures++;
}

//------------------------------------------------------------------
inline int Surface::GetNumTextures() const
{
	return m_iNumTextures;
}

//------------------------------------------------------------------
inline const Texture* Surface::GetTexture(int iIndex) const
{
	return m_ppTextures[iIndex];
}

} // namespace Scene
} // namespace Magnet

#endif