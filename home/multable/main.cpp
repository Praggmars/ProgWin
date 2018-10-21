#include <windows.h>
#include <CommCtrl.h>
#include <string>
#include <cmath>
#include <iostream>

#define ID_MULTIPLIER 1
#define ID_MODULO 2

double PI = 3.14159265358979323846264338327950288419716939937510;
HWND g_mainWindow;
HWND g_multiplierTrackBar;
HWND g_moduloTrackBar;

double g_multiplier;
int g_modulo;

void Event_Paint(HWND hwnd)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	int cx, cy, r;
	int yoffset = 60;
	hdc = BeginPaint(hwnd, &ps);
	GetClientRect(hwnd, &rect);
	rect.bottom -= yoffset;
	r = min(rect.right, rect.bottom) * 10 / 22;
	cx = rect.right / 2;
	cy = rect.bottom / 2 + yoffset;

	for (int i = 0; i < g_modulo; i++)
	{
		double a = (double)i / (double)g_modulo * 2.0 * PI;
		MoveToEx(hdc, cx - r * cos(a), cy - r * sin(a), NULL);
		a = (double)(i*g_multiplier) / (double)g_modulo * 2.0 * PI;
		LineTo(hdc, cx - r * cos(a), cy - r * sin(a));
	}

	EndPaint(hwnd, &ps);
}
void Event_Create(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	g_multiplierTrackBar = CreateWindowEx(
		0, TRACKBAR_CLASS, L"Multiplier",
		WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE,
		0, 0, rect.right, 30, hwnd, (HMENU)ID_MULTIPLIER, GetModuleHandle(NULL), NULL);
	SendMessage(g_multiplierTrackBar, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, rect.right));
	g_moduloTrackBar = CreateWindowEx(
		0, TRACKBAR_CLASS, L"Modulo",
		WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE,
		0, 30, rect.right, 30, hwnd, (HMENU)ID_MODULO, GetModuleHandle(NULL), NULL);
	SendMessage(g_moduloTrackBar, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(1, rect.right + 1));
	g_multiplier = SendMessage(g_multiplierTrackBar, TBM_GETPOS, 0, 0) / 10.0;
	g_modulo = SendMessage(g_moduloTrackBar, TBM_GETPOS, 0, 0);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
		Event_Create(hwnd);
		return 0;
	case WM_HSCROLL:
		g_multiplier = SendMessage(g_multiplierTrackBar, TBM_GETPOS, 0, 0) / 10.0;
		g_modulo = SendMessage(g_moduloTrackBar, TBM_GETPOS, 0, 0);
		InvalidateRect(hwnd, NULL, TRUE);
		break;
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
	const WCHAR appName[] = L"MulTableApp";
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
	g_mainWindow = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, appName, L"MulTable", WS_OVERLAPPEDWINDOW,
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