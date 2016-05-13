#include <assert.h>
#include <list>
#include "embree2\rtcore.h"
#include "embree2\rtcore_ray.h"
#include "embree2\rtcore_scene.h"

#include "Scene\IRenderObject.h"
#include "Scene\Material.h"
#include "Scene\RenderObject.h"
#include "Scene\ResourceManager.h"
#include "Scene\Scene.h"
#include "Scene\SceneManager.h"
#include "Scene\SceneLoader.h"
#include "Scene\Surface.h"
#include "Scene\Mesh.h"

struct EmbreeVertex
{
	float x;
	float y;
	float z;
	float w;
};

struct EmbreeTriangle
{
	int v0;
	int v1;
	int v2;
};


int main()
{
	// system initialization
	Magnet::Scene::ResourceManager::Initialize();
	Magnet::Scene::SceneManager::Initialize();

	Magnet::Scene::SceneManager& sceneManager = Magnet::Scene::SceneManager::GetInstance();
	Magnet::Scene::ResourceManager& resourceManager = Magnet::Scene::ResourceManager::GetInstance();

	Magnet::Scene::SceneLoader* pSceneLoader = sceneManager.GetSceneLoader();
	Magnet::Scene::Scene* pScene = sceneManager.GetScene();

	if (!pSceneLoader->IsLoadingFinished())
	{
		if (pScene == 0)
		{
			pScene = new Magnet::Scene::Scene("C:\\Projects\\GitHub\\LightBaker\\data\\shadowtest.xml");
		}

		pSceneLoader->StartLoadingScene(pScene);
	}

	// load mesh, texture resources
	resourceManager.Process();

	RTCDevice device = rtcNewDevice(NULL);
	RTCScene embreeScene = rtcDeviceNewScene(device, RTC_SCENE_STATIC, RTC_INTERSECT1);

	assert(pSceneLoader->IsLoadingFinished());

	// scenegraph traversal
	pScene->Update(0.f);

	std::list<Magnet::Scene::IRenderObject*>& renderObjectList = pScene->GetRenderObjectList();
	std::list<Magnet::Scene::IRenderObject*>::iterator itEnd = renderObjectList.end();
	for (std::list<Magnet::Scene::IRenderObject*>::iterator it = renderObjectList.begin(); it != itEnd; it++)
	{
		Magnet::Scene::IRenderObject* pRenderObject = *it;

		if (pRenderObject->GetType() == Magnet::Scene::RENDEROBJECT_NORMAL)
		{
			Magnet::Scene::RenderObject* pNormalRenderObject = static_cast<Magnet::Scene::RenderObject*>(pRenderObject);
			if (pNormalRenderObject != NULL)
			{
				const std::list<Magnet::Scene::Surface*>& surfaceList = pNormalRenderObject->GetSurfaceList();

				std::list<Magnet::Scene::Surface*>::const_iterator itSurfaceEnd = surfaceList.end();

				for (std::list<Magnet::Scene::Surface*>::const_iterator itSurface = surfaceList.begin(); itSurface != itSurfaceEnd; ++itSurface)
				{
					const Magnet::Scene::Surface* pSurface = *itSurface;
					const Magnet::Scene::IGeometry* pGeo = pSurface->GetGeometry();

					if (pGeo->GetGeometryType() == Magnet::Scene::GEOMETRY_MESH)
					{
						const Magnet::Scene::Mesh* pMesh = static_cast<const Magnet::Scene::Mesh*>(pGeo);

						const int iStride = pMesh->GetStride();
						const float* pVertexDataBuffer = static_cast<const float*>(pMesh->GetVertexBufferPtr());
						const unsigned int* pIndexDataBuffer = static_cast<const unsigned int*>(pMesh->GetIndexBuffePtr());

						const int iNumVertices = pMesh->GetNumVerts();
						const int iNumTriangles = pMesh->GetNumFaces();

						unsigned int meshID = rtcNewTriangleMesh(embreeScene, RTC_GEOMETRY_STATIC, iNumTriangles, iNumVertices);

						EmbreeVertex* vertices = (EmbreeVertex*)rtcMapBuffer(embreeScene, meshID, RTC_VERTEX_BUFFER);
						for (int i = 0; i < iNumVertices; ++i)
						{
							int iIndex = iStride * i;
							vertices[i].x = pVertexDataBuffer[iIndex];
							vertices[i].y = pVertexDataBuffer[iIndex + 1];
							vertices[i].z = pVertexDataBuffer[iIndex + 2];
						}
						rtcUnmapBuffer(embreeScene, meshID, RTC_VERTEX_BUFFER);

						EmbreeTriangle* triangles = (EmbreeTriangle*)rtcMapBuffer(embreeScene, meshID, RTC_INDEX_BUFFER);
						for (int i = 0; i < iNumTriangles; ++i)
						{
							int iIndex = i * 3;
							triangles[i].v0 = pIndexDataBuffer[iIndex];
							triangles[i].v1 = pIndexDataBuffer[iIndex];
							triangles[i].v2 = pIndexDataBuffer[iIndex];
						}
						rtcUnmapBuffer(embreeScene, meshID, RTC_INDEX_BUFFER);

						//const Magnet::Scene::IMaterial* pMaterial = pSurface->GetMaterial();
						//if (pMaterial->GetType() == Magnet::Scene::MATERIAL_NORMAL)
						//{
							//const Magnet::Scene::Material* pNomalMaterial = static_cast<const Magnet::Scene::Material*>(pMaterial);

						//}
					}
				}
			}
		}
	}

	rtcCommit(embreeScene);

	// compute light maps


	// clean the scene
	rtcDeleteScene(embreeScene);
	rtcDeleteDevice(device);

	// system termination
	Magnet::Scene::SceneManager::Terminate();
	Magnet::Scene::ResourceManager::Terminate();

	return 0;
}
