#ifndef TEXTURE_H
#define TEXTURE_H

namespace Magnet
{

namespace Scene
{
enum TextureFormat
{
	R8G8B8A8_UINT,
	R8G8B8A8_UNORM
};

enum SamplerMode
{
	NOMIP_LINEAR_WRAP,
	NOMIP_LINEAR_UNWRAP,
	NOMIP_ANISO2X_WRAP,
	NOMIP_ANISO2X_UNWRAP,
	MIP_LINEAR_WRAP,
	MIP_LINEAR_UNWRAP
};

class Texture
{
public:
	Texture(const char* pName);
	Texture(const char* pName, int iWidth, int iHeight, TextureFormat eFormat);
	~Texture();

	void SetDimension(int iWidth, int iHeight, int iDepth = 0);
	void SetFormat(TextureFormat eFormat);

	TextureFormat GetFormat() const;
	SamplerMode GetSamplerMode() const;
	int GetWidth() const;
	int GetHeight() const;

	void* CreateDataBuffer();
	void DestroyDataBuffer();
	const char* GetName() const;
	const void* GetDataBufferPtr() const;

	bool IsLoaded()const;
	void SetLoaded(bool bIsLoaded);

private:
	char m_name[256];
	int m_iWidth;
	int m_iHeight;
	TextureFormat m_eFormat;
	SamplerMode m_eSamplerMode;
	void* m_pData;
	bool m_bLoaded;
};

inline void Texture::SetDimension(int iWidth, int iHeight, int iDepth)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;
}

inline void Texture::SetFormat(TextureFormat eFormat)
{
	m_eFormat = eFormat;
}

inline TextureFormat Texture::GetFormat() const
{
	return m_eFormat;
}

inline SamplerMode Texture::GetSamplerMode() const
{
	return m_eSamplerMode;
}

inline int Texture::GetWidth() const
{
	return m_iWidth;
}

inline int Texture::GetHeight() const
{
	return m_iHeight;
}

} // namespace Scene
} // namespace Magnet

#endif