#include "newtonapp.h"

namespace ntn
{
	std::complex<double> NewtonApp::Funct(std::complex<double> z)
	{
		return z * z*z - 1.0;
	}
	std::complex<double> NewtonApp::Deriv(std::complex<double> z)
	{
		return  3.0*z*z;
	}
	void NewtonApp::CreateScreen()
	{
		m_screen = new COLORREF*[m_height];
		*m_screen = new COLORREF[m_height*m_width];
		memset(*m_screen, 0, sizeof(COLORREF)*m_height*m_width);
		for (LONG i = 1; i < m_height; i++)
			m_screen[i] = &(m_screen[i - 1][m_width]);
	}
	void NewtonApp::DeleteScreen()
	{
		if (m_screen)
		{
			delete[] * m_screen;
			delete[] m_screen;
			m_screen = nullptr;
		}
	}
	COLORREF NewtonApp::IterToColor(LONG iter)
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
	void NewtonApp::Paint()
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
	void NewtonApp::DrawNewton()
	{
		double diff;
		double tolerance = 1e-6;
		std::complex<double> roots[3] = {
			std::complex<double>(1.0, 0.0),
			std::complex<double>(-0.5, sqrt(3.0) / 2.0),
			std::complex<double>(-0.5, -sqrt(3.0) / 2.0)
		};
		LONG i;
		for (LONG x = 0; x < m_width; x++)
		{
			for (LONG y = 0; y < m_height; y++)
			{
				std::complex<double> z(X(x), Y(y));
				for (i = 0; i < m_maxIter; i++)
				{
					z -= m_a * Funct(z) / Deriv(z);
					diff = abs(z - roots[0]);
					if (diff < tolerance) break;
					diff = abs(z - roots[1]);
					if (diff < tolerance) break;
					diff = abs(z - roots[2]);
					if (diff < tolerance) break;
				}
				m_screen[y][x] = IterToColor(i);
			}
		}
	}
	NewtonApp::NewtonApp(HWND hwnd) :AppBase(hwnd)
	{
		m_a = 1.0;
		m_maxIter = 100;
		m_centerX = 0.0f;
		m_centerY = 0.0f;
		m_zoom = 0.005;
		m_width = 640;
		m_height = 480;
		CreateScreen();

		DrawNewton();
		SetWindowSizeTitle(m_width, m_height, L"Newton");
		InvalidateRect(hwnd, NULL, FALSE);
	}
	NewtonApp::~NewtonApp()
	{
		DeleteScreen();
	}
	void NewtonApp::MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case WM_PAINT:
			Paint();
			break;
		case WM_LBUTTONDOWN:
			m_centerX = X(LOWORD(lparam));
			m_centerY = Y(HIWORD(lparam));
			DrawNewton();
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case WM_RBUTTONDOWN:
			m_a = std::complex<double>(X(LOWORD(lparam)), Y(HIWORD(lparam)));
			DrawNewton();
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case WM_MOUSEWHEEL:
			if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
				m_zoom /= 1.1;
			else
				m_zoom *= 1.1;
			DrawNewton();
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		}
	}
	LPCWSTR NewtonApp::HelpDialogText()
	{
		return L"Newton fractal\n\
Left click to jump to new location\n\
Right click to set progresson value\n\
Scroll mouse wheel to zoom in and out";
	}
}