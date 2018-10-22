#include "juliaapp.h"

namespace jul
{
	void JuliaApp::CreateScreen()
	{
		m_screen = new COLORREF*[m_height];
		*m_screen = new COLORREF[m_height*m_width];
		memset(*m_screen, 0, sizeof(COLORREF)*m_height*m_width);
		for (LONG i = 1; i < m_height; i++)
			m_screen[i] = &(m_screen[i - 1][m_width]);
	}
	void JuliaApp::DeleteScreen()
	{
		if (m_screen)
		{
			delete[] * m_screen;
			delete[] m_screen;
			m_screen = nullptr;
		}
	}
	COLORREF JuliaApp::IterToColor(LONG iter)
	{
		double r = double(iter) / double(m_maxIter);
		double R = abs(r * 6 - 3) - 1;
		double G = 2 - abs(r * 6 - 2);
		double B = 2 - abs(r * 6 - 4);
		R *= (1.0 - R * 0.49);
		G *= (1.0 - R * 0.49);
		B *= (1.0 - R * 0.49);
		if (R < 0.0) R = 0.0; if (R > 1.0) R = 1.0;
		if (G < 0.0) G = 0.0; if (G > 1.0) G = 1.0;
		if (B < 0.0) B = 0.0; if (B > 1.0) B = 1.0;
		return RGB(R * 0xff, G * 0xff, B * 0xff);
	}
	void JuliaApp::Paint()
	{
		PAINTSTRUCT ps;
		RECT rect;
		HDC hdc = BeginPaint(m_hwnd, &ps);
		GetClientRect(m_hwnd, &rect);
		HBITMAP hbmp = CreateBitmap(m_width, m_height, 1, 32, *m_screen);
		HDC hmem = CreateCompatibleDC(hdc);
		SelectObject(hmem, hbmp);
		StretchBlt(hdc, 0, 0, rect.right, rect.bottom, hmem, 0, 0, m_width, m_height, SRCCOPY);
		DeleteObject(hmem);
		EndPaint(m_hwnd, &ps);
	}
	void JuliaApp::DrawJulia()
	{
		double zx, zy;
		double tmp;
		LONG i;
		for (LONG x = 0; x < m_width; x++)
		{
			for (LONG y = 0; y < m_height; y++)
			{
				zx = X(x);
				zy = Y(y);
				for (i = 0; i < m_maxIter; i++)
				{
					tmp = zx * zx - zy * zy;
					zy = 2 * zx*zy + m_offsetX;
					zx = tmp + m_offsetY;
					if (zx*zx + zy * zy > 4.0)
						break;
				}
				m_screen[y][x] = IterToColor(i);
			}
		}
	}
	JuliaApp::JuliaApp(HWND hwnd) :AppBase(hwnd)
	{
		m_maxIter = 300;
		m_centerX = 0.0f;
		m_centerY = 0.0f;
		m_zoom = 0.005;
		m_width = 640;
		m_height = 480;
		CreateScreen();

		DrawJulia();
		SetWindowSizeTitle(m_width, m_height, L"Julia");
		InvalidateRect(hwnd, NULL, FALSE);
	}
	JuliaApp::~JuliaApp()
	{
		DeleteScreen();
	}
	void JuliaApp::MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case WM_PAINT:
			Paint();
			break;
		case WM_LBUTTONDOWN:
			m_centerX = X(LOWORD(lparam));
			m_centerY = Y(HIWORD(lparam));
			DrawJulia();
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case WM_RBUTTONDOWN:
			m_offsetX = X(LOWORD(lparam));
			m_offsetY = Y(HIWORD(lparam));
			DrawJulia();
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case WM_MOUSEWHEEL:
			if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
				m_zoom /= 1.1;
			else
				m_zoom *= 1.1;
			DrawJulia();
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		}
	}
	LPCWSTR JuliaApp::HelpDialogText()
	{
		return L"Julia set\n\
Left click to jump to new location\n\
Right click to add offset\n\
Scroll mouse wheel to zoom in and out";
	}
}