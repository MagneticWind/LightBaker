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
	SAMPLER_NOMIP_LINEAR_WRAP,
	SAMPLER_NOMIP_LINEAR_UNWRAP,
	SAMPLER_NOMIP_ANISO2X_WRAP,
	SAMPLER_NOMIP_ANISO2X_UNWRAP,
	SAMPLER_MIP_LINEAR_WRAP,
	SAMPLER_MIP_LINEAR_UNWRAP,

};

// the label will be used in shader
enum TextureLabel
{
	TEXTURE_LABEL_COLOR_0,
	TEXTURE_LABEL_COLOR_1,
	TEXTURE_LABEL_NORMAL,
	TEXTURE_LABEL_SPECULAR,
	TEXTURE_LABEL_EMISSIVE,
	TEXTURE_LABEL_SHADOW,
	TEXTURE_LABEL_DEPTH,
	TEXTURE_LABEL_FRAME,
	MAX_TEXTURE_LABEL_COUNT
};

class Texture
{
public:
	Texture(const char* pName);
	Texture(const char* pName, SamplerMode eSampler, TextureLabel eLabel, TextureFormat eFormat);
	Texture(const char* pName, int iWidth, int iHeight, TextureFormat eFormat);
	~Texture();

	void SetDimension(int iWidth, int iHeight, int iDepth = 0);
	void SetFormat(TextureFormat eFormat);

	TextureFormat GetFormat() const;
	SamplerMode GetSamplerMode() const;
	TextureLabel GetLabel() const;

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
	char m_labelName[256];	// name in shader
	int m_iWidth;
	int m_iHeight;
	TextureFormat m_eFormat;
	SamplerMode m_eSamplerMode;
	TextureLabel m_eLabel;
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

inline TextureLabel Texture::GetLabel() const
{
	return m_eLabel;
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