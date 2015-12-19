#ifndef GPU_RESOURCE_MANAGER_H
#define GPU_RESOURCE_MANAGER_H

#include <map>
#include <string>
#include "GPUResource.h"
#include "Scene\Texture.h"

namespace Magnet
{
namespace Scene
{
class Mesh;
class Texture;
}

namespace HALgfx
{
class IDevice;
class ISamplerState;
}

namespace Renderer
{
class GPUResourceManager
{
private:
	GPUResourceManager();
	~GPUResourceManager();
	GPUResourceManager(const GPUResourceManager&);
	GPUResourceManager& operator=(const GPUResourceManager&);

	static GPUResourceManager* ms_pInstance;

public:
	static GPUResourceManager& GetInstance();
	static void Initialize();
	static bool Exist();
	static void Terminate();

public:
	void CreateMeshResource(const Scene::Mesh* pMesh, HALgfx::IDevice* pDevice);// void* pVertexShaderCode, int iShaderSize);
	void CreateTextureResource(const Scene::Texture* pTexture, HALgfx::IDevice* pDevice);
	void CreateCubeTextureResource(const char* pTextureName, HALgfx::IDevice* pDevice);
	void CreateSamplerState(const Scene::SamplerMode mode, HALgfx::IDevice* pDevice);

	MeshResource& GetMeshResource(const std::string name);
	TextureResource& GetTextureResource(const std::string name);
	HALgfx::ISamplerState* GetSamplerState(int samplerMode);

private:
	std::map<std::string, MeshResource> m_MeshMap;
	std::map<std::string, TextureResource> m_TextureMap;
	std::map<int, HALgfx::ISamplerState*> m_SamplerMap;
};

} // namespace Renderer
} // namespace Magnet

#endif