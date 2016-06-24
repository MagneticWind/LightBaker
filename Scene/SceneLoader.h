#ifndef SCENELOADER_H
#define SCENELOADER_H

#include <stdio.h>
#include "IEntity.h"

namespace tinyxml2
{
	class XMLElement;
}

namespace Magnet
{
class IRenderObject;
class Surface;
class IMaterial;
class Texture;

namespace Scene
{

class IEntity;
class Scene;
class SceneNode;
class Camera;
class Light;
class Entity;

class SceneLoader
{
public:
	SceneLoader();
	void StartLoadingScene(Scene* const pScene);

	//
	bool IsLoadingFinished() const;

private:
	// parser functions
	bool LoadSceneGraph(const char* pSceneFile);

	void ParseSceneNode(tinyxml2::XMLElement* pElement, SceneNode* pParentNode);
	void ParseEntity(tinyxml2::XMLElement* pElement, SceneNode* pNode);
	void ParseCamera(tinyxml2::XMLElement* pElement, Camera* pCamera);
	void ParseLight(tinyxml2::XMLElement* pElement, Light* pLight);
	void ParseEntity(tinyxml2::XMLElement* pElement, Entity* pEntity);
	IRenderObject* ParseRenderObject(tinyxml2::XMLElement* pElement);
	void ParseTransformation(tinyxml2::XMLElement* pElement, IRenderObject* pObject);
	void ParseSurface(tinyxml2::XMLElement* pElement, Surface* pSurface);
	void ParseMaterial(tinyxml2::XMLElement* pElement, IMaterial* pMaterial);
	void ParseTexture(tinyxml2::XMLElement* pElement, Texture* pTexture);

private:
	char m_caSceneName[256];
	Scene* m_pCurrentLoadingScene;
	bool m_bFinishedLoading;
};

//------------------------------------------------------------------
inline bool SceneLoader::IsLoadingFinished() const
{
	return m_bFinishedLoading;
}

} // namespace Scene
} // namespace Magnet
#endif