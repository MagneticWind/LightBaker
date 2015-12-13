#ifndef SCENE_H
#define SCENE_H

#include <list>
#include "SceneNode.h"

namespace Magnet
{
namespace Math
{
class Matrix4f;
}

namespace Scene
{
class Camera;
class Light;
class IEntity;
class SceneNode;
class Entity;

class Scene
{
	friend class SceneLoader;
public:
	Scene(const char* pSceneName);

	const char* GetName() const;
	void SetSceneRoot(SceneNode* pRoot);
	void AddCamera(Camera* pCamera);
	void SetCurrentCamera(Camera* pCamera);
	Camera* GetCurrentCamera();
	Camera* GetQuadCamera();
	void AddLight(Light* pLight);
	void AddEntity(IEntity* pEntity);

	std::list<IEntity*>& GetEntityList();
	std::list<Light*>& GetLightList();
	std::list<IRenderObject*>& GetRenderObjectList();

	// iterate and update all entities
	void Update(float fTimeLapse);

private:
	void TraverseSceneGraph(SceneNode* pNode, Math::Matrix4f& mat);                // traverse scene graph
	void ViewFrustumCulling();
	void InitializeDebugLinesObject();
	void DestroyDebugLineObject();
	void UpdateDebugLinesObject();

private:
	char m_caSceneName[256];
	bool m_bActive;
	Camera* m_pCurrentCamera;
	Camera* m_pQuadCamera;
	SceneNode* m_pSceneRoot;                // scene structure

	std::list<IEntity*> m_lEntities;
	std::list<IRenderObject*> m_lRenderObjects;
	std::list<Camera*> m_lCameras;
	std::list<Light*> m_lLights;
};

//------------------------------------------------------------------
inline const char* Scene::GetName() const
{
	return m_caSceneName;
}

//------------------------------------------------------------------
inline void Scene::SetSceneRoot(SceneNode* pRoot)
{
	m_pSceneRoot = pRoot;
}

//------------------------------------------------------------------
inline void Scene::AddCamera(Camera* pCamera)
{
	m_lCameras.push_back(pCamera);
}

//------------------------------------------------------------------
inline void Scene::SetCurrentCamera(Camera* pCamera)
{
	m_pCurrentCamera = pCamera;
}

//------------------------------------------------------------------
inline Camera* Scene::GetCurrentCamera()
{
	return m_pCurrentCamera;
}

//------------------------------------------------------------------
inline void Scene::AddLight(Light* pLight)
{
	m_lLights.push_back(pLight);
}

//------------------------------------------------------------------
inline void Scene::AddEntity(IEntity* pEntity)
{
	m_lEntities.push_back(pEntity);
}

//------------------------------------------------------------------
inline std::list<IEntity*>& Scene::GetEntityList()
{
	return m_lEntities;
}

//------------------------------------------------------------------
inline std::list<Light*>& Scene::GetLightList()
{
	return m_lLights;
}

//------------------------------------------------------------------
inline std::list<IRenderObject*>& Scene::GetRenderObjectList()
{
	return m_lRenderObjects;
}

} // namespace Scene
} // namespace Magnet
#endif