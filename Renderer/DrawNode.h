#ifndef DRAWNODE_H
#define DRAWNODE_H

#include "Math\Matrix4f.h"
#include "Math\Vector3f.h"
#include "HALgfx\ShaderType.h"
#include "HALgfx\Viewport.h"

namespace Magnet
{
namespace HALgfx
{
class IBuffer;
class ISamplerState;
class IShaderResourceView;
struct Viewport;
}

namespace Scene
{
class Material;
}

namespace Renderer
{
#define MAX_NUMBER_BUFFERS 8
#define MAX_NUMBER_SAMPLERS 8
#define MAX_NUMBER_SRVS 8

struct DrawNode
{
	DrawNode();
	~DrawNode();

	void AddSRV(HALgfx::IShaderResourceView* pSRV);
	void AddSampler(HALgfx::ISamplerState* pSampler);
	void RemoveSRV(HALgfx::IShaderResourceView* pSRV);

	void* CreateCBufferData(int iSize, HALgfx::ShaderType eType);
	void DestroyCBufferData(void* pBuffer);

	// geometry
	HALgfx::IBuffer* m_pVertexBuffer;
	HALgfx::IBuffer* m_pIndexBuffer;

	// c buffers
	void* m_pVSCBufferData[MAX_NUMBER_BUFFERS];
	void* m_pPSCBufferData[MAX_NUMBER_BUFFERS];

	// textures
	HALgfx::IShaderResourceView* m_ppSRVs[MAX_NUMBER_SRVS];

	// 
	HALgfx::ISamplerState* m_ppSamplers[MAX_NUMBER_SAMPLERS];

	char m_caName[256];

	int m_iNumberOfSamplers;
	int m_iNumberOfSRVs;
	int m_iNumberOfPrimitives;
	int m_iNumberOfVSConstBuffers;
	int m_iNumberOfPSConstBuffers;

	bool m_bSetViewport;
	HALgfx::ViewPort mViewport;
};
} // namespace Renderer
} // namespace Magnet

#endif