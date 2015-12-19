#ifndef APPLICATION_H
#define APPLICATION_H

#include <Windows.h>

#include "Timer.h"

class RenderWindow;

// a singleton class
class Application
{
private:
	Application();
	~Application();
	Application(const Application&);
	Application& operator=(const Application&);
	static Application* ms_pInstance;

public:
	static Application& GetInstance();
	static bool Exist();
	static void Initialize();
	static void Terminate();

	static void GetCurrentMousePosition(int& iPositionX, int& iPositionY);

	void InitializeWindow(int iWidth, int iHeight, int iLeft, int iTop, const char* pCaption);
	void DestoryWindow();
	void InitializeSystem(bool bConsole);
	void DestroySystem();

	// get called each frame
	void Update();

	// handle inputs: mouse and keyboard
	void OnLeftButtonDown(LPARAM wParam);
	void OnLeftButtonUp();
	void OnKeyDown(WPARAM wParam);
	void OnKeyUp();

private:
	void InitializeSingletons();
	void TerminateSingletons();

private:
	RenderWindow* m_pRenderWindow;
	bool m_bLog;
	bool m_bConsole;

	Timer m_timer;
	float m_fLastFrameTimeLapse;

	// input parameters
	int m_iMoveForward;
	int m_iMoveLeft;

	int m_iLastMousePositionX;
	int m_iLastMousePositionY;
	bool m_bMouseLButtonDown;

	int m_iIntensityControl;

	bool m_bCallbacksSet;
};



#endif