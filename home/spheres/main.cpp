#include <windows.h>
#include <string>
#include <cmath>
#include <iostream>
#include <array>
#include <vector>

#define ID_TIMER1 1

double PI = 3.14159265358979323846264338327950288419716939937510;
HWND g_mainWindow;
double g_time = 0.0;
const double deltaTime = 0.05;
const int sphereRadius = 36;
const int sphereDistaceHalf = 40;

struct Point
{
	double x;
	double y;
};

std::array<std::array<std::vector<Point>, 9>, 18> g_spheres;

void Event_Paint(HWND hwnd)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	hdc = BeginPaint(hwnd, &ps);
	GetClientRect(hwnd, &rect);

	for (int i = 0; i < g_spheres.size(); i++)
		Ellipse(hdc, sphereDistaceHalf*(3 + i * 2) - sphereRadius, sphereDistaceHalf - sphereRadius,
			sphereDistaceHalf*(3 + i * 2) + sphereRadius, sphereDistaceHalf + sphereRadius);
	for (int i = 0; i < g_spheres[0].size(); i++)
		Ellipse(hdc, sphereDistaceHalf - sphereRadius, sphereDistaceHalf*(3 + i * 2) - sphereRadius,
			sphereDistaceHalf + sphereRadius, sphereDistaceHalf*(3 + i * 2) + sphereRadius);

	for (int x = 0; x < g_spheres.size(); x++)
	{
		for (int y = 0; y < g_spheres[x].size(); y++)
		{
			if (!g_spheres[x][y].empty())
			{
				MoveToEx(hdc, sphereDistaceHalf*(4 + x * 2) - sphereRadius * (1 + g_spheres[x][y][0].x),
					sphereDistaceHalf*(4 + y * 2) - sphereRadius * (1 + g_spheres[x][y][0].y), NULL);
				for (Point p : g_spheres[x][y])
					LineTo(hdc, sphereDistaceHalf*(4 + x * 2) - sphereRadius * (1 + p.x),
						sphereDistaceHalf*(4 + y * 2) - sphereRadius * (1 + p.y));
			}
		}
	}

	EndPaint(hwnd, &ps);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
		SetTimer(hwnd, ID_TIMER1, 20, NULL);
		return 0;
	case WM_TIMER:
		if (LOWORD(wparam) == ID_TIMER1)
		{
			g_time += deltaTime;
			if (g_time < PI * 2.1)
			{
				for (int x = 0; x < g_spheres.size(); x++)
				{
					for (int y = 0; y < g_spheres[x].size(); y++)
					{
						Point p;
						p.x = sin(g_time*(x + 1));
						p.y = cos(g_time*(y + 1));
						g_spheres[x][y].push_back(p);
					}
				}
				InvalidateRect(hwnd, NULL, TRUE);
			}
			else
				KillTimer(hwnd, ID_TIMER1);
		}
	case WM_PAINT:
		Event_Paint(hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR szCMLine, int iCmdShow)
{
	const WCHAR appName[] = L"SpheresApp";
	WNDCLASS wc;
	MSG msg;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = appName;

	RegisterClass(&wc);
	g_mainWindow = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, appName, L"Spheres", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, GetModuleHandle(NULL), NULL);

	ShowWindow(g_mainWindow, SW_SHOW);
	UpdateWindow(g_mainWindow);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}