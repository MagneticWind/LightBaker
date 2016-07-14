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
	m_eType = TEXTURE_TYPE_2D;

	m_bLoaded = false;
	m_pData = NULL;
}

//------------------------------------------------------------------
Texture::Texture(const char* pName, SamplerMode eSampler, TextureLabel eLabel, TextureFormat eFormat, TextureType eType)
{
	strcpy(m_name, pName);

	m_eSamplerMode = eSampler;
	m_eLabel = eLabel;
	m_eFormat = eFormat;
	m_eType = eType;

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
	m_eType = TEXTURE_TYPE_2D;

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
	int perFaceByteSize = 0;

	switch (m_eFormat)
	{
	case TEXTURE_FORMAT_R8G8B8A8_UINT:
	case TEXTURE_FORMAT_R8G8B8A8_UNORM:
		perFaceByteSize = 4 * m_iWidth * m_iHeight;
		break;
	case TEXTURE_FORMAT_R32G32B32A32_FLOAT:
		perFaceByteSize = 16 * m_iWidth * m_iHeight;
		break;
	default:
		perFaceByteSize = 0;
		break;
	}

	switch (m_eType)
	{
	case TEXTURE_TYPE_2D:
		m_pData = malloc(perFaceByteSize);
		break;
	case TEXTURE_TYPE_CUBE:
		m_pData = malloc(perFaceByteSize * 6);
		break;
	default:
		m_pData = 0;
		break;
	}

	return m_pData;
}

void Texture::DestroyDataBuffer()
{
	free(m_pData);
}

} // namespace Scene
} // namespace Magnet