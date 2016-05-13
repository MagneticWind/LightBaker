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
				m_pScene = new Scene("C:\\Projects\\GitHub\\LightBaker\\data\\shadowtest.xml");
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

	//------------------------------------------------------------------
	//void SceneManager::UpdateCurrentCameraParams(const Camera::CameraMoveParameter& cameraParam)
	//{
	//	m_pScene->GetCurrentCamera()->UpdateCameraParam(cameraParam);
	//}


} // namespace Scene
} // namespace Magnet