#include <string.h>

#include "RenderWindow.h"

// the definition is in main.cpp
extern LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

//------------------------------------------------------------------
RenderWindow::RenderWindow()
{
}

//------------------------------------------------------------------
RenderWindow::~RenderWindow()
{
}

//------------------------------------------------------------------
int RenderWindow::GetWidth() const
{
	RECT rect;
	GetClientRect(m_hWnd, &rect);
	return(rect.right - rect.left);
}

//------------------------------------------------------------------
int RenderWindow::GetHeight() const
{
	RECT rect;
	GetClientRect(m_hWnd, &rect);
	return(rect.bottom - rect.top);
}

//------------------------------------------------------------------
int RenderWindow::GetLeft() const
{
	POINT point;
	point.x = 0;
	point.y = 0;
	ClientToScreen(m_hWnd, &point);
	return(point.x);
}

//------------------------------------------------------------------
int RenderWindow::GetTop() const
{
	POINT point;
	point.x = 0;
	point.y = 0;
	ClientToScreen(m_hWnd, &point);
	return(point.y);
}

//------------------------------------------------------------------
void RenderWindow::SetWidth(const int width)
{
	m_iWidth = width;
	UpdateWindowState();
}

//------------------------------------------------------------------
void RenderWindow::SetHeight(const int height)
{
	m_iHeight = height;
	UpdateWindowState();
}

//------------------------------------------------------------------
void RenderWindow::SetSize(const int width, const int height)
{
	m_iWidth = width;
	m_iHeight = height;
	UpdateWindowState();
}

//------------------------------------------------------------------
void RenderWindow::SetPosition(const int left, const int top)
{
	m_iLeft = left;
	m_iTop = top;
	UpdateWindowState();
}

//------------------------------------------------------------------
void RenderWindow::SetCaption(const char* pCaption)
{
	strcpy(m_csCaption, pCaption);
	if (m_hWnd != 0)
		SetWindowText(m_hWnd, pCaption);
}

//------------------------------------------------------------------
void RenderWindow::UpdateWindowState()
{
	if (m_hWnd != 0)
	{
		RECT ClientRect;
		ClientRect.left = 0;
		ClientRect.top = 0;
		ClientRect.right = m_iWidth;
		ClientRect.bottom = m_iHeight;

		// Adjust the window size for correct device size
		RECT WindowRect = ClientRect;
		AdjustWindowRect(&WindowRect, m_dStyle, FALSE);

		int deltaX = (WindowRect.right - ClientRect.right) / 2;
		int deltaY = (WindowRect.bottom - ClientRect.right) / 2;

		MoveWindow(m_hWnd, m_iLeft - deltaX, m_iTop - deltaY,
			m_iWidth + deltaX * 2, m_iHeight + deltaY * 2, true);
	}
}

void RenderWindow::Initialize(int iWidth, int iHeight, int iLeft, int iTop, const char* pCaption)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_iLeft = iLeft;
	m_iTop = iTop;

	SetCaption(pCaption);

	WNDCLASSEX wc;

	// Setup the window class
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = 0;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Magnet";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// Register the window class
	RegisterClassEx(&wc);

	// Specify the window style
	m_dStyle = (WS_OVERLAPPEDWINDOW | WS_VISIBLE) & ~WS_THICKFRAME;

	// Record the desired device size
	RECT rc;
	rc.top = rc.left = 0;
	rc.right = m_iWidth;
	rc.bottom = m_iHeight;

	// Adjust the window size for correct device size
	AdjustWindowRect(&rc, m_dStyle, FALSE);

	long lwidth = rc.right - rc.left;
	long lheight = rc.bottom - rc.top;

	long lleft = (long)m_iLeft;
	long ltop = (long)m_iTop;


	// Create an instance of the window
	m_hWnd = CreateWindowEx(
		NULL,							// extended style
		"Magnet",						// class name
		m_csCaption,					// instance title
		m_dStyle,						// window style
		lleft, ltop,						// initial x,y
		lwidth,							// initial width
		lheight,						// initial height
		NULL,							// handle to parent 
		NULL,							// handle to menu
		NULL,							// instance of this application
		NULL);							// extra creation parms

	// Initially display the window
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);
}

//--------------------------------------------------------------------------------
void RenderWindow::Shutdown()
{
	if (m_hWnd)
		DestroyWindow(m_hWnd);
	m_hWnd = 0;
}

//--------------------------------------------------------------------------------
void RenderWindow::Paint()
{

}
