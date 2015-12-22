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
class IShader;
class IBuffer;
class IDevice;
class IDeviceContext;
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
	ShaderNode(const char* pName);
	~ShaderNode();

	void BindDrawNodeResource(HALgfx::IDeviceContext* pDeviceContext, DrawNode& drawNode);
	void UnbindDrawNodeResource(HALgfx::IDeviceContext* pDeviceContext, DrawNode& pDrawNode);
	void Draw(HALgfx::IDeviceContext* pDeviceContext);
	void ClearDrawNodes();

	void AddDrawNode(DrawNode& drawNode);
	void AddConstantBuffer(const HALgfx::BufferDesc& desc, HALgfx::IDevice* pDevice, HALgfx::ShaderType eType);
	void SetSamplerStates(HALgfx::ISamplerState* pSamplerStates [], int iNumSamplerStates);

	const char* GetName() const;
	void CreateInputLayout(int iNumElements, HALgfx::InputElementDesc inputElements[], HALgfx::IDevice* pDevice);

	// load compiled shades, create cbuffer, input layout and such
	void LoadShader(HALgfx::ShaderType eType);
	void Create(HALgfx::IDevice* pDevice);
	void* CreateBuffer(int iSize, HALgfx::ShaderType eType);
	bool IsLoaded() const;

	DrawNode& GetDrawNode(const char* name);

private:
	char m_caShaderName[256];
	void* m_ppFileData[HALgfx::MAX_SHADER_NUM];
	int m_iFileSize[HALgfx::MAX_SHADER_NUM];
	bool m_bIsLoaded;

	// shaders
	HALgfx::IShader* m_ppShader[HALgfx::MAX_SHADER_NUM];

	// input layout
	HALgfx::IInputLayout* m_pInputLayout;

	// const buffers
	HALgfx::IBuffer* m_ppVSConstBuffers[MAX_NUMBER_BUFFERS];
	HALgfx::IBuffer* m_ppPSConstBuffers[MAX_NUMBER_BUFFERS];
	int m_aVSConstBufferSizes[MAX_NUMBER_BUFFERS];
	int m_aPSConstBufferSizes[MAX_NUMBER_BUFFERS];
	int m_iNumberOfVSConstBuffers;
	int m_iNumberOfPSConstBuffers;

	// samplers
	HALgfx::ISamplerState* m_ppSamplers[MAX_NUMBER_SAMPLERS];
	int m_iNumSamplerState;

	// list of draw nodes
	std::list<DrawNode> m_lDrawNodes;
};

inline const char* ShaderNode::GetName() const
{
	return m_caShaderName;
}

} // namespace Renderer
} // namespace Magnet

#endif