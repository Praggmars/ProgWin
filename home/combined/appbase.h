#pragma once

#include <Windows.h>

class AppBase
{
protected:
	HWND m_hwnd;

protected:
	void SetWindowTitle(const WCHAR title[]);
	void SetWindowSizeTitle(int w, int h, const WCHAR title[]);

public:
	AppBase(HWND hwnd);
	virtual ~AppBase();
	virtual void MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) = 0;
};