#include <assert.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Device_gl.h"
#include "DeviceContext_gl.h"
#include "Program_gl.h"
#include "VertexShader_gl.h"
#include "PixelShader_gl.h"

namespace Magnet
{
namespace HALgfx
{
GLProgram::GLProgram(const char* pName)
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

GLProgram::~GLProgram()
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

void GLProgram::LoadShader(ShaderType eType)
{
	static const char SHADER_PATH[256] = "C:\\Projects\\GitHub\\LightBaker\\shader\\gl\\";
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
			strcat(vShaderPath, ".vert");

			std::ifstream fstream(vShaderPath);
			if (!fstream.is_open())
			{
				assert(0);
			}

			fstream.seekg(0, std::ios::end);
			int iVSFileSize = fstream.tellg();
			fstream.seekg(0, std::ios::beg);

			char* pVSFileData = static_cast<char*>( CreateBuffer(iVSFileSize, HALgfx::VERTEX_SHADER));
			fstream.getline(pVSFileData, iVSFileSize, '\0');

			fstream.close();
			break;
		}
		case HALgfx::PIXEL_SHADER:
		{
			// load pixel shader
			char pShaderPath[256];
			strcpy(pShaderPath, filePath);
			strcat(pShaderPath, ".frag");

			std::ifstream fstream(pShaderPath);
			if (!fstream.is_open())
			{
				assert(0);
			}

			fstream.seekg(0, std::ios::end);
			int iPSFileSize = fstream.tellg();
			fstream.seekg(0, std::ios::beg);

			char* pVSFileData = static_cast<char*>(CreateBuffer(iPSFileSize, HALgfx::PIXEL_SHADER));
			fstream.getline(pVSFileData, iPSFileSize, '\0');

			fstream.close();
			break;
		}
	}

	m_bIsLoaded[eType] = true;
}

void GLProgram::CreateShaders(int iNumElements, HALgfx::InputElementDesc inputElements[], IDevice* pDevice)
{
	m_program = glCreateProgram();

	for (int i = 0; i < MAX_SHADER_NUM; ++i)
	{
		if (m_bIsLoaded[i])
		{
			m_ppShader[i] = pDevice->CreateShader(static_cast<HALgfx::ShaderType>(i), m_iFileSize[i], m_ppFileData[i]);
			
			if (i == VERTEX_SHADER)
			{
				GLVertexShader* pShader = static_cast<GLVertexShader*>(m_ppShader[i]);
				glAttachShader(m_program, pShader->GetGLId());
			}
			else if (i == PIXEL_SHADER)
			{
				GLPixelShader* pShader = static_cast<GLPixelShader*>(m_ppShader[i]);
				glAttachShader(m_program, pShader->GetGLId());
			}
		}
	}

	// set attribute locations for vertex shader: shader needs to mathch vertex buffer
	for (int i = 0; i < iNumElements; ++i)
	{
		glBindAttribLocation(m_program, i, inputElements[i].semantic);
	}

	glLinkProgram(m_program);

}

void GLProgram::SetShaders(int iNumTextureLabels, int textureLabels[], IDeviceContext* pDeviceContext)
{
	GLDeviceContext* deviceContext = static_cast<GLDeviceContext*>(pDeviceContext);
	deviceContext->SetCurrentProgram(m_program);
	glUseProgram(m_program);

	//enum TextureLabel
	//{
	//	TEXTURE_LABEL_COLOR_0,
	//	TEXTURE_LABEL_COLOR_1,
	//	TEXTURE_LABEL_NORMAL,
	//	TEXTURE_LABEL_SPECULAR,
	//	TEXTURE_LABEL_EMISSIVE,
	//	TEXTURE_LABEL_SHADOW,
	//	TEXTURE_LABEL_DEPTH,
	//	TEXTURE_LABEL_FRAME,
	//	MAX_TEXTURE_LABEL_COUNT
	//};
	for (int i = 0; i < iNumTextureLabels; ++i)
	{
		switch (textureLabels[i])
		{
			case 0:
			{
				GLint color0 = glGetUniformLocation(m_program, "txColor0");
				glUniform1i(color0, i);
				break;
			}
			case 1:
			{
				GLint color1 = glGetUniformLocation(m_program, "txColor1");
				glUniform1i(color1, i);
			}
			case 2:
			{
				GLint txNormal = glGetUniformLocation(m_program, "txNormal");
				glUniform1i(txNormal, i);
				break;
			}
			case 3:
			{
				GLint txSpecular = glGetUniformLocation(m_program, "txSpecular");
				glUniform1i(txSpecular, i);
				break;
			}
			case 4:
			{
				GLint txEmissive = glGetUniformLocation(m_program, "txEmissive");
				glUniform1i(txEmissive, i);
				break;
			}
			case 5:
			{
				GLint txShadow = glGetUniformLocation(m_program, "txShadow");
				glUniform1i(txShadow, i);
				break;
			}
			case 6:
			{
				GLint txDepth = glGetUniformLocation(m_program, "txDepth");
				glUniform1i(txDepth, i);
				break;
			}
			case 7:
			{
				GLint txFrame = glGetUniformLocation(m_program, "txFrame");
				glUniform1i(txFrame, i);
				break;
			}
		}
	}
}

void GLProgram::ClearShaders(IDeviceContext* pDeviceContext)
{
	GLDeviceContext* deviceContext = static_cast<GLDeviceContext*>(pDeviceContext);
	deviceContext->SetCurrentProgram(0);
	glUseProgram(0);
}

void* GLProgram::CreateBuffer(int iSize, HALgfx::ShaderType eType)
{
	m_ppFileData[eType] = malloc(iSize);
	m_iFileSize[eType] = iSize;
	return m_ppFileData[eType];
}

}
}