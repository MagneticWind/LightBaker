#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Device_plat.h"
#include "DeviceContext_plat.h"
#include "Program_plat.h"

namespace Magnet
{
namespace HALgfx
{
Program::Program(const char* pName)
{
	strcpy(m_name, pName);
	for (int i = 0; i < MAX_SHADER_NUM; ++i)
	{
		m_ppShader[i] = 0;
		m_ppFileData[i] = 0;
		m_iFileSize[i] = 0;
		m_bIsLoaded[i] = false;
	}
}

Program::~Program()
{
	for (int i = 0; i < HALgfx::MAX_SHADER_NUM; ++i)
	{
		if (m_ppFileData[i])
		{
			free(m_ppFileData[i]);
			m_ppFileData[i] = 0;
		}

		if (m_ppShader[i])
		{
			delete m_ppShader[i];
			m_ppShader[i] = 0;
		}
	}
}

void Program::LoadShader(ShaderType eType)
{
	static const char SHADER_PATH[256] = "C:\\Projects\\GitHub\\LightBaker\\data\\shader\\";
	char filePath[256];
	strcpy(filePath, SHADER_PATH);
	strcat(filePath, m_name);

	switch (eType)
	{
	case HALgfx::VERTEX_SHADER:
	{
		// load vertex shader
		char vShaderPath[256];
		strcpy(vShaderPath, filePath);
		strcat(vShaderPath, ".v");

		FILE* pVSFile = fopen(vShaderPath, "r+b");
		if (pVSFile == 0)
		{
			printf("can't find the shader %s", vShaderPath);
			assert(0);
		}

		fseek(pVSFile, 0, SEEK_END);
		int iVSFileSize = ftell(pVSFile);
		rewind(pVSFile);

		void* pVSFileData = CreateBuffer(iVSFileSize, HALgfx::VERTEX_SHADER);
		int iResult = fread(pVSFileData, 1, iVSFileSize, pVSFile);
		if (iResult != iVSFileSize)
		{
			printf("Error reading shader file %s", pVSFileData);
			assert(0);
		}

		fclose(pVSFile);
	}
	break;
	case HALgfx::PIXEL_SHADER:
	{
		// load pixel shader
		char pShaderPath[256];
		strcpy(pShaderPath, filePath);
		strcat(pShaderPath, ".p");

		FILE* pPSFile = fopen(pShaderPath, "r+b");
		if (pPSFile == 0)
		{
			printf("can't find the shader %s", pShaderPath);
			assert(0);
		}

		fseek(pPSFile, 0, SEEK_END);
		int iPSFileSize = ftell(pPSFile);
		rewind(pPSFile);

		void* pPSFileData = CreateBuffer(iPSFileSize, HALgfx::PIXEL_SHADER);
		int iSize = fread(pPSFileData, 1, iPSFileSize, pPSFile);
		if (iSize != iPSFileSize)
		{
			printf("Error reading shader file %s", pShaderPath);
			assert(0);
		}

		fclose(pPSFile);
	}
	break;
	case HALgfx::COMPUTE_SHADER:
	{
		// load compute shader
		char pShaderPath[256];
		strcpy(pShaderPath, filePath);
		strcat(pShaderPath, ".c");

		FILE* pCSFile = fopen(pShaderPath, "r+b");
		if (pCSFile == 0)
		{
			printf("can't find the shader %s", pShaderPath);
			assert(0);
		}

		fseek(pCSFile, 0, SEEK_END);
		int iCSFileSize = ftell(pCSFile);
		rewind(pCSFile);

		void* pCSFileData = CreateBuffer(iCSFileSize, HALgfx::COMPUTE_SHADER);
		int iSize = fread(pCSFileData, 1, iCSFileSize, pCSFile);
		if (iSize != iCSFileSize)
		{
			printf("Error reading shader file %s", pShaderPath);
			assert(0);
		}

		fclose(pCSFile);
	}
	break;
	}

	m_bIsLoaded[eType] = true;
}

void Program::CreateShaders(int iNumElements, HALgfx::InputElementDesc inputElements[], IDevice* pDevice)
{
	for (int i = 0; i < MAX_SHADER_NUM; ++i)
	{
		if (m_bIsLoaded[i])
		{
			m_ppShader[i] = pDevice->CreateShader(static_cast<HALgfx::ShaderType>(i), m_iFileSize[i], m_ppFileData[i]);
		}
	}
}

void Program::SetShaders(IDeviceContext* pDeviceContext)
{
	for (int i = 0; i < MAX_SHADER_NUM; ++i)
	{
		pDeviceContext->SetShader(static_cast<HALgfx::ShaderType>(i), m_ppShader[i]);
	}
}

void Program::ClearShaders(IDeviceContext* pDeviceContext)
{
	for (int i = 0; i < MAX_SHADER_NUM; ++i)
	{
		pDeviceContext->SetShader(static_cast<HALgfx::ShaderType>(i), 0);
	}
}

void* Program::CreateBuffer(int iSize, HALgfx::ShaderType eType)
{
	m_ppFileData[eType] = malloc(iSize);
	m_iFileSize[eType] = iSize;
	return m_ppFileData[eType];
}

}
}