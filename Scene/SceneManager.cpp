#include <assert.h>

#include "Camera.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "SceneLoader.h"
#include "Scene.h"
#include "Surface.h"


namespace Magnet
{
namespace Scene
{

	SceneManager* SceneManager::ms_pInstance = 0;

	//------------------------------------------------------------------
	SceneManager::SceneManager()
	{
		m_pScene = 0;
		m_pScreenSpaceCamera = 0;
		m_bSceneLoaded = false;
		m_pSceneLoader = new SceneLoader();
		//CreateScreenSpaceCamera();
		//CreateQuad();
	}

	//------------------------------------------------------------------
	SceneManager::~SceneManager()
	{
		delete m_pSceneLoader;
	}

	//------------------------------------------------------------------
	SceneManager& SceneManager::GetInstance()
	{
		assert(ms_pInstance != 0);
		return *ms_pInstance;
	}

	//------------------------------------------------------------------
	void SceneManager::Initialize()
	{
		assert(ms_pInstance == 0);
		ms_pInstance = new SceneManager();
	}

	//------------------------------------------------------------------
	bool SceneManager::Exist()
	{
		return ms_pInstance != 0;
	}

	//------------------------------------------------------------------
	void SceneManager::Terminate()
	{
		delete ms_pInstance;
		ms_pInstance = 0;
	}

	//------------------------------------------------------------------
	void SceneManager::Update(float fTimeLapse)
	{
		// scene loading
		if (!m_pSceneLoader->IsLoadingFinished())
		{
			if (m_pScene == 0)
			{
				m_pScene = new Scene("C:\\Projects\\GitHub\\LightBaker\\data\\cornell_box.xml");
			}

			m_pSceneLoader->StartLoadingScene(m_pScene);
		}

		// Resource Manager
		ResourceManager::GetInstance().Process();

		// update scene structure, based on the current view, put all visible entities for render
		m_pScene->Update(fTimeLapse);

	}

	// called by application\inputsystem
	void SceneManager::UpdateCurrentCameraRotationParams(bool bRotate, int iLastMousePositionX, int iLastMousePositionY)
	{
		m_pScene->GetCurrentCamera()->SetRotationParams(bRotate, iLastMousePositionX, iLastMousePositionY);
	}

	void SceneManager::UpdateCurrentCameraTranslationParams(bool bTranslate, int iMoveForward, int iMoveLeft)
	{
		m_pScene->GetCurrentCamera()->SetTranslationParams(bTranslate, iMoveForward, iMoveLeft);
	}

	void SceneManager::SetCurrentCameraGetMousePositionCallback(CallbackGetCurrentMousePosition callbackGetMousePosition)
	{
		m_pScene->GetCurrentCamera()->SetGetCurrentMousePositionCallback(callbackGetMousePosition);
	}

#if 0
	//------------------------------------------------------------------
	void SceneManager::CreateScreenSpaceCamera()
	{
		m_pScreenSpaceCamera = new Camera(0, false);
		m_pScreenSpaceCamera->SetPosition(Vector3f(0.f, 0.f, 1.f));
		m_pScreenSpaceCamera->SetLookat(Vector3f(0.f, 0.f, 0.f));
		m_pScreenSpaceCamera->SetUpVector(Vector3f(0.f, 1.f, 0.f));
		m_pScreenSpaceCamera->SetNear(0.1f);
		m_pScreenSpaceCamera->SetFar(2.f);
		m_pScreenSpaceCamera->UpdateViewProjectMatrices();
	}


	//------------------------------------------------------------------
	void SceneManager::CreateQuad()
	{
		Vertex aVertices[4];
		aVertices[0].v3Position = Vector3f(-1.f, 1.f, 0.f);
		aVertices[1].v3Position = Vector3f(-1.f, -1.f, 0.f);
		aVertices[2].v3Position = Vector3f(1.f, -1.f, 0.f);
		aVertices[3].v3Position = Vector3f(1.f, 1.f, 0.f);

		aVertices[0].v3Normal = Vector3f(0.f, 0.f, 1.f);
		aVertices[1].v3Normal = Vector3f(0.f, 0.f, 1.f);
		aVertices[2].v3Normal = Vector3f(0.f, 0.f, 1.f);
		aVertices[3].v3Normal = Vector3f(0.f, 0.f, 1.f);

		aVertices[0].v2TexCoord = Vector2f(1.f, 0.f);
		aVertices[1].v2TexCoord = Vector2f(1.f, 1.f);
		aVertices[2].v2TexCoord = Vector2f(0.f, 1.f);
		aVertices[3].v2TexCoord = Vector2f(0.f, 0.f);

		m_pQuadRenderObject = new NormalObject();
		TriangleMesh* pTriangleMesh = new TriangleMesh(false);
		for (int i = 0; i < 4; ++i)
			pTriangleMesh->AddVertex(aVertices[i]);
		pTriangleMesh->AddTriangle(0, 1, 2);
		pTriangleMesh->AddTriangle(0, 2, 3);

		RenderUnit* pRenderUnit = new RenderUnit(static_cast<IGeometry*>(pTriangleMesh));
		m_pQuadRenderObject->AddRenderUnit(pRenderUnit);

	}
#endif

	//------------------------------------------------------------------
	//void SceneManager::UpdateCurrentCameraParams(const Camera::CameraMoveParameter& cameraParam)
	//{
	//	m_pScene->GetCurrentCamera()->UpdateCameraParam(cameraParam);
	//}


} // namespace Scene
} // namespace Magnet