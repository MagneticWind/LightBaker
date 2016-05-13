#include <assert.h>
#include <stdio.h>

//#define USE_OCULUS_VR

#include "Application.h"
#include "RenderWindow.h"

#ifdef USE_OCULUS_VR
#include "Renderer\RenderManagerOVR.h"
#else
#include "Renderer\RenderManager.h"
#endif

#include "Scene\SceneManager.h"
#include "Scene\ResourceManager.h"

Application* Application::ms_pInstance = 0;

//------------------------------------------------------------------
Application::Application()
{
	m_pRenderWindow = 0;
	m_fLastFrameTimeLapse = 0.f;

	m_bCallbacksSet = false;

	m_iMoveForward = 0;
	m_iMoveLeft = 0;

	m_iIntensityControl = 0;
}

//------------------------------------------------------------------
Application::~Application()
{
}

//------------------------------------------------------------------
Application& Application::GetInstance()
{
	assert(ms_pInstance != 0);
	return *ms_pInstance;
}

//------------------------------------------------------------------
bool Application::Exist()
{
	return ms_pInstance != 0;
}

//------------------------------------------------------------------
void Application::Initialize()
{
	assert(ms_pInstance == 0);
	ms_pInstance = new Application();
}

//------------------------------------------------------------------
void Application::GetCurrentMousePosition(int& iPositionX, int& iPositionY)
{
	POINT ptCurMousePos;
	GetCursorPos(&ptCurMousePos);
	iPositionX = ptCurMousePos.x;
	iPositionY = ptCurMousePos.y;
}

//------------------------------------------------------------------
void Application::InitializeWindow(int iWidth, int iHeight, int iLeft, int iTop, const char* pCaption)
{
	if (ms_pInstance)
	{
		m_pRenderWindow = new RenderWindow();
		if (m_pRenderWindow)
			m_pRenderWindow->Initialize(iWidth, iHeight, iLeft, iTop, pCaption);
	}
}

//------------------------------------------------------------------
void Application::DestoryWindow()
{
	if (m_pRenderWindow)
	{
		m_pRenderWindow->Shutdown();
		delete m_pRenderWindow;
	}
}

//------------------------------------------------------------------
void Application::InitializeSystem(bool bConsole)
{
	m_bConsole = bConsole;

	InitializeSingletons();
}

//------------------------------------------------------------------
void Application::DestroySystem()
{
	TerminateSingletons();
}

//------------------------------------------------------------------
void Application::Terminate()
{
	delete ms_pInstance;
	ms_pInstance = 0;
}

//------------------------------------------------------------------
void Application::Update()
{
	m_timer.Start();

	if (m_fLastFrameTimeLapse > 0.f)
	{
		printf("<LOG>: Application::Update...framerate: %f fps. \n", 1.f / m_fLastFrameTimeLapse);
	}

	// simulation
	Magnet::Scene::SceneManager::GetInstance().Update(m_fLastFrameTimeLapse);

	if (!m_bCallbacksSet)
	{
		Magnet::Scene::SceneManager::GetInstance().SetCurrentCameraGetMousePositionCallback(GetCurrentMousePosition);
		m_bCallbacksSet = true;
	}

	// render
#ifdef USE_OCULUS_VR
	Magnet::Renderer::RenderManagerOVR::GetInstance().RenderOneFrame();
#else
	Magnet::Renderer::RenderManager::GetInstance().RenderOneFrame();
#endif

	m_fLastFrameTimeLapse = m_timer.GetElapsedSeconds();
	m_timer.Stop();
}

//------------------------------------------------------------------
void Application::OnLeftButtonDown(LPARAM wParam)
{
	//m_iPreviousX = (short)LOWORD(lParam);
	//m_iPreviousY = (short)HIWORD(lParam);
	m_bMouseLButtonDown = true;
	POINT mousePosition;
	GetCursorPos(&mousePosition);

	Magnet::Scene::SceneManager::GetInstance().UpdateCurrentCameraRotationParams(true, mousePosition.x, mousePosition.y);
}

//------------------------------------------------------------------
void Application::OnLeftButtonUp()
{
	//m_iMoveX = 0;
	//m_iMoveY = 0;
	//m_iRotX = 0;
	//m_iRotY = 0;
	m_bMouseLButtonDown = false;
	POINT mousePosition;
	GetCursorPos(&mousePosition);
	Magnet::Scene::SceneManager::GetInstance().UpdateCurrentCameraRotationParams(false, 0, 0);
}

//------------------------------------------------------------------
void Application::OnKeyDown(WPARAM wParam)
{
	switch (wParam)
	{
	case 'W':
	case 'w':
		m_iMoveForward = 1;
		break;
	case 'S':
	case 's':
		m_iMoveForward = -1;
		break;
	case 'A':
	case 'a':
		m_iMoveLeft = 1;
		break;
	case 'D':
	case 'd':
		m_iMoveLeft = -1;
		break;
	case VK_UP:
		{
			break;
		}
	case VK_DOWN:
		{
			break;
		}
	}

	Magnet::Scene::SceneManager::GetInstance().UpdateCurrentCameraTranslationParams(true, m_iMoveForward, m_iMoveLeft);
}

//------------------------------------------------------------------
void Application::OnKeyUp()
{
	m_iMoveForward = 0;
	m_iMoveLeft = 0;
	Magnet::Scene::SceneManager::GetInstance().UpdateCurrentCameraTranslationParams(false, m_iMoveForward, m_iMoveLeft);
}

//------------------------------------------------------------------
void Application::InitializeSingletons()
{
	Magnet::Scene::ResourceManager::Initialize();
	Magnet::Scene::SceneManager::Initialize();

#ifdef USE_OCULUS_VR
	Magnet::Renderer::RenderManagerOVR::Initialize(m_pRenderWindow->GetWidth(), m_pRenderWindow->GetHeight(),
		m_pRenderWindow->GetHandle());
#else
	Magnet::Renderer::RenderManager::Initialize(m_pRenderWindow->GetWidth(), m_pRenderWindow->GetHeight(),
		m_pRenderWindow->GetHandle());
#endif
}

//------------------------------------------------------------------
void Application::TerminateSingletons()
{
#ifdef USE_OCULUS_VR
	Magnet::Renderer::RenderManagerOVR::Terminate();
#else
	Magnet::Renderer::RenderManager::Terminate();
#endif

	Magnet::Scene::SceneManager::Terminate();
	Magnet::Scene::ResourceManager::Terminate();
}

