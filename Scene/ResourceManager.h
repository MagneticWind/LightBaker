#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <list>

namespace Magnet
{
namespace Scene
{
class Mesh;
class Texture;
class IGeometry;

// a singleton class
class ResourceManager
{
private:
	ResourceManager();
	~ResourceManager();
	ResourceManager(const ResourceManager&);
	ResourceManager& operator=(const ResourceManager&);
	static ResourceManager* ms_pInstance;

public:
	static ResourceManager& GetInstance();
	static bool Exist();
	static void Initialize();
	static void Terminate();

	void Process();
	void LoadResources();

	void SetTextureFolderPath(const char* pFolderPath);
	void SetMeshFolderPath(const char* pFolderPath);
	const char* GetTextureFolderPath() const;
	const char* GetMeshFolderPath() const;
	void AddMesh(const char* pMeshName, bool bNormalize, bool bBinary);
	void AddTexture(Texture* pTexture);

	static void LoadMesh(Mesh* pMesh);
	static void LoadTexture(Texture* pTexture);

	Texture* FindTexture(const char* pTextureName);
	IGeometry* FindGeometry(const char* pGeometryName);

	void AddGeometry(IGeometry* pGeometry);

private:
	static void LoadMeshPly(Mesh* pMesh);
	static void LoadMeshObj(Mesh* pMesh);

private:
	std::list<IGeometry*> m_lGeometrys;
	std::list<Texture*> m_lTextures;

	char m_caMeshFolderPath[256];        // folder path of meshes
	char m_caTextureFolderPath[256];     // folder path of textures
};

inline const char* ResourceManager::GetTextureFolderPath() const
{
	return m_caTextureFolderPath;
}

inline const char* ResourceManager::GetMeshFolderPath() const
{
	return m_caMeshFolderPath;
}

} // namespace Scene
} // namespace Magnet

#endif