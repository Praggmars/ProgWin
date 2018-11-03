#include "appbase.h"

AppBase::AppBase(HWND hwnd) :m_hwnd(hwnd) {}
AppBase::~AppBase() {}

void AppBase::SetWindowTitle(const WCHAR title[])
{
	SetWindowText(m_hwnd, title);
}

void AppBase::SetWindowSizeTitle(int w, int h, const WCHAR title[])
{
	RECT clientRect;
	clientRect.left = 0;
	clientRect.right = w;
	clientRect.top = 0;
	clientRect.bottom = h;
	AdjustWindowRectEx(&clientRect, WS_OVERLAPPEDWINDOW, TRUE, WS_EX_OVERLAPPEDWINDOW);
	RECT windowRect;
	GetWindowRect(m_hwnd, &windowRect);
	windowRect.right = windowRect.left + clientRect.right - clientRect.left;
	windowRect.bottom = windowRect.top + clientRect.bottom - clientRect.top;
	MoveWindow(m_hwnd, windowRect.left, windowRect.top,
		windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, TRUE);
	SetWindowText(m_hwnd, title);
}