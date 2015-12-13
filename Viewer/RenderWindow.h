#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <windows.h>

#define NUM_CHAR_CAPTION 64

class RenderWindow
{
public:
	RenderWindow();
	~RenderWindow();

	HWND GetHandle() const;
	const char* GetCaption() const;
	int GetWidth() const;
	int GetHeight() const;
	int GetLeft() const;
	int GetTop() const;

	void SetCaption(const char* pCaption);
	void SetWidth(const int width);
	void SetHeight(const int height);
	void SetSize(const int width, const int height);
	void SetPosition(const int left, const int top);

	void Initialize(int iWidth, int iHeight, int iLeft, int iTop, const char* pCaption);
	void Shutdown();
	void Paint();

	//int GetSwapChain();
	//void SetSwapChain( int swapchain );
private:
	void UpdateWindowState();

private:
	HWND m_hWnd;
	char m_csCaption[NUM_CHAR_CAPTION];
	int m_iWidth;
	int m_iHeight;
	int m_iLeft;
	int m_iTop;
	//int m_iSwapChain;
	DWORD m_dStyle;
};

//------------------------------------------------------------------
inline HWND RenderWindow::GetHandle() const
{
	return m_hWnd;
}

//------------------------------------------------------------------
inline const char* RenderWindow::GetCaption() const
{
	return m_csCaption;
}

#endif