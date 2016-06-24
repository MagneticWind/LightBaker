#include "Texture.h"
#include <string.h>
#include <stdlib.h>

namespace Magnet
{
namespace Scene
{
//------------------------------------------------------------------
Texture::Texture(const char* pName)
{
	strcpy(m_name, pName);

	m_eSamplerMode = SAMPLER_NOMIP_LINEAR_WRAP;

	m_bLoaded = false;
	m_pData = NULL;
}

//------------------------------------------------------------------
Texture::Texture(const char* pName, SamplerMode eSampler, TextureLabel eLabel, TextureFormat eFormat)
{
	strcpy(m_name, pName);

	m_eSamplerMode = eSampler;
	m_eLabel = eLabel;
	m_eFormat = eFormat;

	m_bLoaded = false;
	m_pData = NULL;
}

//------------------------------------------------------------------
Texture::Texture(const char* pName, int iWidth, int iHeight, TextureFormat eFormat)
{
	strcpy(m_name, pName);
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_eFormat = eFormat;

	m_eSamplerMode = SAMPLER_NOMIP_LINEAR_WRAP;

	m_bLoaded = false;
	m_pData = NULL;
}

//------------------------------------------------------------------
Texture::~Texture()
{
	if (m_pData)
	{
		free(m_pData);
		m_pData = 0;
	}
}

//------------------------------------------------------------------
const char* Texture::GetName() const
{
	return m_name;
}

//------------------------------------------------------------------
const void* Texture::GetDataBufferPtr() const
{
	return m_pData;
}

//------------------------------------------------------------------
bool Texture::IsLoaded() const
{
	return m_bLoaded;
}

//------------------------------------------------------------------
void Texture::SetLoaded(bool bIsLoaded)
{
	m_bLoaded = bIsLoaded;
}

//------------------------------------------------------------------
void* Texture::CreateDataBuffer()
{
	switch (m_eFormat)
	{
	case R8G8B8A8_UINT:
	case R8G8B8A8_UNORM:
		m_pData = malloc(32 * m_iWidth * m_iHeight);
		return m_pData;
	default:
		break;
	}
}

void Texture::DestroyDataBuffer()
{
	free(m_pData);
}

} // namespace Scene
} // namespace Magnet