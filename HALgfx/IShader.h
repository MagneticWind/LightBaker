#ifndef ISHADER_H
#define ISHADER_H

#include "IInputLayout.h"
#include "ShaderType.h"

namespace Magnet
{
namespace HALgfx
{
class IDevice;
class IDeviceContext;

class IShader
{
public:
	virtual ~IShader(){}
};

class IProgram
{
public:
	virtual void LoadShader(ShaderType eType) = 0;
	virtual void CreateShaders(int iNumElements, HALgfx::InputElementDesc inputElements[], IDevice* pDevice) = 0;
	virtual void SetShaders(int iNumTextureLabels, int textureLabels[], IDeviceContext* pDeviceContext) = 0;
	virtual void ClearShaders(IDeviceContext* pDeviceContext) = 0;

	const char* GetName() const
	{
		return m_name;
	}

	const bool IsLoaded() const
	{
		return m_bIsLoaded;
	}

	const int GetFileSize(ShaderType eType) const
	{
		return m_iFileSize[eType];
	}

	const void* GetFileData(ShaderType eType) const
	{
		return m_ppFileData[eType];
	}

	IShader* GetShader(ShaderType eType)
	{
		return m_ppShader[eType];
	}

protected:
	char m_name[256];
	IShader* m_ppShader[MAX_SHADER_NUM];
	void* m_ppFileData[HALgfx::MAX_SHADER_NUM];
	int m_iFileSize[HALgfx::MAX_SHADER_NUM];
	bool m_bIsLoaded[HALgfx::MAX_SHADER_NUM];
};
} // namespace HALgfx
} // namespace Magnet

#endif