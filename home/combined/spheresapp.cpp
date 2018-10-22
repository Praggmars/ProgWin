#include "spheresapp.h"
#include "linalg.h"

#define ID_TIMER1 1

namespace sph
{
	constexpr double g_deltaTime = 0.05;
	constexpr int g_sphereRadius = 36;
	constexpr int g_sphereDistaceHalf = 40;

	void SpheresApp::DrawNextLines()
	{
		for (size_t x = 0; x < m_spheres.size(); x++)
		{
			for (size_t y = 0; y < m_spheres[x].size(); y++)
			{
				Point p;
				p.x = sin(m_time*(x + 1));
				p.y = cos(m_time*(y + 1));
				m_spheres[x][y].push_back(p);
			}
		}
		InvalidateRect(m_hwnd, NULL, TRUE);
	}

	void SpheresApp::Paint()
	{
		HDC hdc;
		PAINTSTRUCT ps;
		RECT rect;
		HBRUSH hbrush = CreateSolidBrush(0x0);
		HPEN hpen = CreatePen(PS_SOLID, 1, RGB(0xff, 0xff, 0xff));
		hdc = BeginPaint(m_hwnd, &ps);
		GetClientRect(m_hwnd, &rect);
		SelectObject(hdc, hbrush);
		SelectObject(hdc, hpen);

		for (size_t i = 0; i < m_spheres.size(); i++)
			Ellipse(hdc, g_sphereDistaceHalf*(3 + i * 2) - g_sphereRadius, g_sphereDistaceHalf - g_sphereRadius,
				g_sphereDistaceHalf*(3 + i * 2) + g_sphereRadius, g_sphereDistaceHalf + g_sphereRadius);
		for (size_t i = 0; i < m_spheres[0].size(); i++)
			Ellipse(hdc, g_sphereDistaceHalf - g_sphereRadius, g_sphereDistaceHalf*(3 + i * 2) - g_sphereRadius,
				g_sphereDistaceHalf + g_sphereRadius, g_sphereDistaceHalf*(3 + i * 2) + g_sphereRadius);

		for (size_t x = 0; x < m_spheres.size(); x++)
		{
			for (size_t y = 0; y < m_spheres[x].size(); y++)
			{
				if (!m_spheres[x][y].empty())
				{
					MoveToEx(hdc, g_sphereDistaceHalf*(4 + x * 2) - (int)(g_sphereRadius * (1 + m_spheres[x][y][0].x)),
						g_sphereDistaceHalf*(4 + y * 2) - (int)(g_sphereRadius * (1 + m_spheres[x][y][0].y)), NULL);
					for (Point p : m_spheres[x][y])
						LineTo(hdc, g_sphereDistaceHalf*(4 + x * 2) - (int)(g_sphereRadius * (1 + p.x)),
							g_sphereDistaceHalf*(4 + y * 2) - (int)(g_sphereRadius * (1 + p.y)));
				}
			}
		}
		DeleteObject(hbrush);
		DeleteObject(hpen);
		EndPaint(m_hwnd, &ps);
	}

	SpheresApp::SpheresApp(HWND hwnd) :AppBase(hwnd)
	{
		SetTimer(hwnd, ID_TIMER1, 20, NULL);
		m_time = 0.0;
		SetWindowTitle(L"Spheres");
	}
	SpheresApp::~SpheresApp()
	{
		KillTimer(m_hwnd, ID_TIMER1);
	}
	void SpheresApp::MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case WM_TIMER:
			if (LOWORD(wparam) == ID_TIMER1)
			{
				m_time += g_deltaTime;
				if (m_time < linalg::PI * 2.1)
					DrawNextLines();
				else
					KillTimer(hwnd, ID_TIMER1);
			}
			break;
		case WM_PAINT:
			Paint();
			break;
		}
	}
	LPCWSTR SpheresApp::HelpDialogText()
	{
		return L"Drawing lines with changing x and y coordinates";
	}
}