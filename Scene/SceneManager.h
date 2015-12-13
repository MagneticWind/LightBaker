#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Camera.h"

namespace Magnet
{
	class IRenderObject;

namespace Scene
{
	class SceneLoader;
	class Scene;

	// a singleton class
	class SceneManager
	{
	private:
		SceneManager();
		~SceneManager();
		SceneManager(const SceneManager&);
		SceneManager& operator=(const SceneManager&);
		static SceneManager* ms_pInstance;

	public:
		static SceneManager& GetInstance();
		static bool Exist();
		static void Initialize();
		static void Terminate();

		Scene* GetScene();

		// Simulation
		void Update(float fTimeLapse);

		// called by application\inputsystem
		void UpdateCurrentCameraRotationParams(bool bRotate, int iLastMousePositionX, int iLastMousePositionY);
		void UpdateCurrentCameraTranslationParams(bool bTranslate, int iMoveForward, int iMoveLeft);
		void SetCurrentCameraGetMousePositionCallback(CallbackGetCurrentMousePosition callbackGetMousePosition);



	private:
		//void CreateScreenSpaceCamera();
		//void CreateQuad();

	private:
		SceneLoader* m_pSceneLoader;
		Scene* m_pScene;
		Camera* m_pScreenSpaceCamera;
		IRenderObject* m_pQuadRenderObject;
		bool m_bSceneLoaded;
	};

	inline Scene* SceneManager::GetScene()
	{
		return m_pScene;
	}

} // namespace Scene
} // namespace Magnet
#endif