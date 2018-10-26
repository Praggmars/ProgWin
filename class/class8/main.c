#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <math.h>

#define ID_TIMER1 100
#define ID_BUTTON1 10
#define ID_BUTTON2 11

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc2(HWND, UINT, WPARAM, LPARAM);

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR szCmdLine, INT iCmdShow)
{
	static WCHAR szAppName[] = L"winapp";
	HWND        hwnd;
	MSG         msg;
	WNDCLASSEX  wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(
		szAppName, L"Controller", WS_OVERLAPPEDWINDOW,
		700, 200, 500, 500,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	WNDCLASSEX wndclass2;
	static HWND hwnd2;

	switch (iMsg)
	{
	case WM_CREATE:
		wndclass2.cbSize = sizeof(wndclass2);
		wndclass2.style = CS_HREDRAW | CS_VREDRAW;
		wndclass2.lpfnWndProc = WndProc2;
		wndclass2.cbClsExtra = 0;
		wndclass2.cbWndExtra = 0;
		wndclass2.hInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
		wndclass2.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass2.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass2.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wndclass2.lpszMenuName = NULL;
		wndclass2.lpszClassName = L"win2";
		wndclass2.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		RegisterClassEx(&wndclass2);
		hwnd2 = CreateWindow(L"win2", L"Spin", WS_OVERLAPPEDWINDOW,
			200, 200, 500, 500, NULL, NULL, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
		ShowWindow(hwnd2, SW_SHOW);
		UpdateWindow(hwnd2);

		CreateWindow(L"button", L"Start", WS_CHILD | WS_VISIBLE, 30, 30, 200, 40, hwnd,
			(HMENU)ID_BUTTON1, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
		CreateWindow(L"button", L"Stop", WS_CHILD | WS_VISIBLE, 30, 80, 200, 40, hwnd,
			(HMENU)ID_BUTTON2, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
		return 0;

	case WM_COMMAND:
		if (wParam == ID_BUTTON1)
			SetTimer(hwnd2, ID_TIMER1, 20, NULL);
		else
			KillTimer(hwnd2, ID_TIMER1);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

LRESULT CALLBACK WndProc2(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static double time = 0;
	switch (iMsg)
	{
	case WM_TIMER:
		if (wParam == ID_TIMER1)
		{
			HDC hdc = GetDC(hwnd);
			RECT rect;
			GetClientRect(hwnd, &rect);
			PatBlt(hdc, 0, 0, rect.right, rect.bottom, BLACKNESS);
			Ellipse(hdc, (int)(rect.right / 2 * (1 - (sin(time)))), 0, (int)(rect.right / 2 * (1 + (sin(time)))), rect.bottom);
			MoveToEx(hdc, rect.right / 2, 0, NULL);
			LineTo(hdc, rect.right / 2, rect.bottom);
			ReleaseDC(hwnd, hdc);
			time += 0.05;
		}
		return 0;
	case WM_CREATE:
		SetTimer(hwnd, ID_TIMER1, 20, NULL);
		return 0;
	case WM_DESTROY:
		DestroyWindow(hwnd);
		return 0;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}