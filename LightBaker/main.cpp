#include <list>
#include "embree2\rtcore.h"
#include "embree2\rtcore_ray.h"
#include "embree2\rtcore_scene.h"

#include "Scene\IRenderObject.h"
#include "Scene\ResourceManager.h"
#include "Scene\Scene.h"
#include "Scene\SceneManager.h"
#include "Scene\SceneLoader.h"


int main()
{
	// system initialization
	Magnet::Scene::ResourceManager::Initialize();
	Magnet::Scene::SceneManager::Initialize();

	Magnet::Scene::SceneManager& sceneManager = Magnet::Scene::SceneManager::GetInstance();
	Magnet::Scene::SceneLoader* pSceneLoader = sceneManager.GetSceneLoader();
	Magnet::Scene::Scene* pScene = sceneManager.GetScene();

	if (!pSceneLoader->IsLoadingFinished())
	{
		if (pScene == 0)
		{
			pScene = new Magnet::Scene::Scene("C:\\Projects\\GitHub\\LightBaker\\data\\cornell_box.xml");
		}

		pSceneLoader->StartLoadingScene(pScene);
	}

	// Magnet scene to Embree scene
	RTCDevice device = rtcNewDevice(NULL);

	RTCScene scene = rtcDeviceNewScene(device, RTC_SCENE_STATIC, RTC_INTERSECT1);

	rtcDeleteScene(scene);

	rtcDeleteDevice(device);

	// system termination
	Magnet::Scene::SceneManager::Terminate();
	Magnet::Scene::ResourceManager::Terminate();

	return 0;
}
