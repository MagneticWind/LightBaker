#ifndef PROGRAM_H
#define PROGRAM_H

#include "IShader.h"

namespace Magnet
{
namespace HALgfx
{
class IDevice;
class Program : public IProgram
{
public:
	Program(const char* name);
	~Program();
	virtual void LoadShader(ShaderType eType);
	virtual void CreateShaders(int iNumElements, HALgfx::InputElementDesc inputElements[], IDevice* pDevice);
	virtual void SetShaders(IDeviceContext* pDeviceContext);
	virtual void ClearShaders(IDeviceContext* pDeviceContext);
private:
	void* CreateBuffer(int iSize, ShaderType eType);
};
}
}

#endif