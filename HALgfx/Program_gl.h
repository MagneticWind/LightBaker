#ifndef GL_PROGRAM_H
#define GL_PROGRAM_H

#include "glew.h"		// has to be included before gl.h
#include <gl/gl.h>
#include <gl/glu.h>

#include "IShader.h"

namespace Magnet
{
namespace HALgfx
{
class IDevice;
class IDeviceContext;

class GLProgram : public IProgram
{
public:
	GLProgram(const char* name);
	~GLProgram();
	virtual void LoadShader(ShaderType eType);
	virtual void CreateShaders(int iNumElements, HALgfx::InputElementDesc inputElements[], IDevice* pDevice);
	virtual void SetShaders(IDeviceContext* pDeviceContext);
	virtual void ClearShaders(IDeviceContext* pDeviceContext);

private:
	void* CreateBuffer(int iSize, ShaderType eType);

private:
	GLuint m_program;
};
}
}

#endif