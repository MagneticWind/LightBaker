#ifndef SHADER_NODE_H
#define SHADER_NODE_H

#include <list>

#include "HALgfx\ShaderType.h"
#include "DrawNode.h"

namespace Magnet
{

namespace HALgfx
{
class IInputLayout;
class IProgram;
class IBuffer;
class IDevice;
class IDeviceContext;
class IShaderResourceView;
class IUnorderedAccessView;
struct BufferDesc;
}

namespace Renderer
{
class Surface;
class Texture;
class Shader;
class ShaderNode;
struct DrawNode;

bool ShaderNodeExists(const char* pName, std::list<ShaderNode*>& shaderNodeList, ShaderNode** ppReturnNode);

class ShaderNode
{
public:
	ShaderNode(const char* pName, HALgfx::IDevice* pDevice);
	~ShaderNode();

	void BindDrawNodeResource(HALgfx::IDeviceContext* pDeviceContext, DrawNode& drawNode);
	void UnbindDrawNodeResource(HALgfx::IDeviceContext* pDeviceContext, DrawNode& pDrawNode);
	void Draw(HALgfx::IDeviceContext* pDeviceContext);
	void ClearDrawNodes();

	void AddDrawNode(DrawNode& drawNode);
	void AddConstantBuffer(const HALgfx::BufferDesc& desc, HALgfx::IDevice* pDevice, HALgfx::ShaderType eType);

	const char* GetName() const;
	void CreateInputLayout(int iNumElements, HALgfx::InputElementDesc inputElements[], HALgfx::IDevice* pDevice);

	// load compiled shades, create cbuffer, input layout and such
	void LoadShader(HALgfx::ShaderType eType);
	void Create(int iNumElements, HALgfx::InputElementDesc inputElements[], HALgfx::IDevice* pDevice);
	void* CreateBuffer(int iSize, HALgfx::ShaderType eType);

	void AddTextureLabel(int iLabel);

	// compute shader
	void RunCompute(HALgfx::IDeviceContext* pDeviceContext, int iSRVCount, HALgfx::IShaderResourceView** ppSRVs, 
		void* CBufferData, int iUAVCount, HALgfx::IUnorderedAccessView** pUAVs, unsigned int uX, unsigned int uY, unsigned uZ);

	DrawNode& GetDrawNode(const char* name);

private:
	ShaderNode();

	// shader program
	HALgfx::IProgram* m_pShaderProgram;

	// input layout
	HALgfx::IInputLayout* m_pInputLayout;
	int m_iNumElements;
	HALgfx::InputElementDesc m_inputElements[MAX_NUM_ELEMENTS];

	// const buffers
	HALgfx::IBuffer* m_ppVSConstBuffers[MAX_NUMBER_BUFFERS];
	HALgfx::IBuffer* m_ppPSConstBuffers[MAX_NUMBER_BUFFERS];
	HALgfx::IBuffer* m_ppCSConstBuffers[MAX_NUMBER_BUFFERS];
	int m_aVSConstBufferSizes[MAX_NUMBER_BUFFERS];
	int m_aPSConstBufferSizes[MAX_NUMBER_BUFFERS];
	int m_aCSConstBufferSizes[MAX_NUMBER_BUFFERS];
	int m_iNumberOfVSConstBuffers;
	int m_iNumberOfPSConstBuffers;
	int m_iNumberOfCSConstBuffers;

	// texture labels
	int m_iNumTextureLabels;
	int m_textureLabels[MAX_NUMBER_SRVS];

	// list of draw nodes
	std::list<DrawNode> m_lDrawNodes;
};

} // namespace Renderer
} // namespace Magnet

#endif