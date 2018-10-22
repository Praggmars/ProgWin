#include "multableapp.h"
#include "linalg.h"
#include <CommCtrl.h>

#define ID_MULTIPLIER 1
#define ID_MODULO 2

namespace mtbl
{
	void MultableApp::Paint()
	{
		HDC hdc;
		PAINTSTRUCT ps;
		RECT rect;
		HPEN hp = CreatePen(PS_SOLID, 1, RGB(0xff, 0xff, 0xff));
		int cx, cy, r;
		int yoffset = 60;
		hdc = BeginPaint(m_hwnd, &ps);
		SelectObject(hdc, hp);
		GetClientRect(m_hwnd, &rect);
		rect.bottom -= yoffset;
		r = min(rect.right, rect.bottom) * 10 / 22;
		cx = rect.right / 2;
		cy = rect.bottom / 2 + yoffset;

		for (int i = 0; i < m_modulo; i++)
		{
			double a = (double)i / (double)m_modulo * 2.0 * linalg::PI;
			MoveToEx(hdc, (int)(cx - r * cos(a)), (int)(cy - r * sin(a)), NULL);
			a = (double)(i*m_multiplier) / (double)m_modulo * 2.0 * linalg::PI;
			LineTo(hdc, (int)(cx - r * cos(a)), (int)(cy - r * sin(a)));
		}
		DeleteObject(hp);

		EndPaint(m_hwnd, &ps);
	}
	MultableApp::MultableApp(HWND hwnd) :AppBase(hwnd)
	{
		RECT rect;
		GetClientRect(hwnd, &rect);
		m_multiplierTrackBar = CreateWindowEx(
			0, TRACKBAR_CLASS, L"Multiplier",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE,
			0, 0, rect.right, 30, hwnd, (HMENU)ID_MULTIPLIER, GetModuleHandle(NULL), NULL);
		SendMessage(m_multiplierTrackBar, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, rect.right));
		m_moduloTrackBar = CreateWindowEx(
			0, TRACKBAR_CLASS, L"Modulo",
			WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE,
			0, 30, rect.right, 30, hwnd, (HMENU)ID_MODULO, GetModuleHandle(NULL), NULL);
		SendMessage(m_moduloTrackBar, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(1, rect.right + 1));
		m_multiplier = SendMessage(m_multiplierTrackBar, TBM_GETPOS, 0, 0) / 10.0;
		m_modulo = SendMessage(m_moduloTrackBar, TBM_GETPOS, 0, 0);
		SetWindowTitle(L"MulTable");
		InvalidateRect(hwnd, NULL, TRUE);
	}
	MultableApp::~MultableApp()
	{
		DestroyWindow(m_moduloTrackBar);
		DestroyWindow(m_multiplierTrackBar);
	}
	void MultableApp::MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case WM_HSCROLL:
			m_multiplier = SendMessage(m_multiplierTrackBar, TBM_GETPOS, 0, 0) / 10.0;
			m_modulo = SendMessage(m_moduloTrackBar, TBM_GETPOS, 0, 0);
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case WM_PAINT:
			Paint();
			break;
		}
	}
	LPCWSTR MultableApp::HelpDialogText()
	{
		return L"Modulo multiplication\n\
Move upper trackbar to set multiplier\n\
Move lower trackbar to set modulo";
	}
}