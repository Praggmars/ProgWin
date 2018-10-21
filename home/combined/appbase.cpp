#include "appbase.h"

AppBase::AppBase(HWND hwnd) :m_hwnd(hwnd) {}
AppBase::~AppBase() {}

void AppBase::SetWindowTitle(const WCHAR title[])
{
	SetWindowText(m_hwnd, title);
}

void AppBase::SetWindowSizeTitle(int w, int h, const WCHAR title[])
{
	RECT rect;
	rect.left = 0;
	rect.right = w;
	rect.top = 0;
	rect.bottom = h;
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, TRUE, WS_EX_OVERLAPPEDWINDOW);
	MoveWindow(m_hwnd,
		(GetSystemMetrics(SM_CXSCREEN) - rect.right + rect.left) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - rect.bottom + rect.top) / 2,
		rect.right - rect.left, rect.bottom - rect.top, TRUE);
	SetWindowText(m_hwnd, title);
}