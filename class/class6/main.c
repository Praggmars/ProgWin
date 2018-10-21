#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR szCmdLine, INT iCmdShow)
{
	static WCHAR szAppName[] = L"213";
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
		szAppName,
		L"213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213    213"
		, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		/*if (msg.message == WM_KEYDOWN)
			++msg.wParam;*/
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	HFONT hf;
	LOGFONT lf;
	WCHAR buf[128];
	int i;
	static int fs = 60;

	switch (iMsg)
	{
	case WM_MOUSEWHEEL:
		hdc = GetDC(hwnd);
		GetClientRect(hwnd, &rect);
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			++fs;
		else
			--fs;

		lf.lfHeight = fs;
		lf.lfWidth = fs / 2;
		lf.lfEscapement = 0;
		lf.lfUnderline = FALSE;
		lf.lfStrikeOut = FALSE;
		lf.lfItalic = FALSE;
		lf.lfWeight = 0x7fffffff;
		lstrcpyW(lf.lfFaceName, L"consolas");
		hf = CreateFontIndirect(&lf);
		SelectObject(hdc, hf);
		SetTextColor(hdc, RGB(0, 0, 255));
		TextOut(hdc, rect.right / 2, rect.bottom / 2, L"213", 3);
		DeleteObject(hf);
		ReleaseDC(hwnd, hdc);
		return 0;

	case WM_CREATE:
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);

		lf.lfHeight = 60;
		lf.lfWidth = 30;
		lf.lfEscapement = 0;
		lf.lfUnderline = FALSE;
		lf.lfStrikeOut = FALSE;
		lf.lfItalic = FALSE;
		lf.lfWeight = 0x7fffffff;
		lstrcpyW(lf.lfFaceName, L"consolas");
		hf = CreateFontIndirect(&lf);
		SelectObject(hdc, hf);
		SetTextColor(hdc, RGB(0, 0, 255));
		for (i = 0; i < 213; i++)
			TextOut(hdc, rand() % rect.right, rand() % rect.bottom, L"213", 3);
		DeleteObject(hf);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CHAR:
		hdc = GetDC(hwnd);
		wsprintf(buf, L"Virtual keycode: %d ANSI: %c   ", wParam, wParam);
		TextOut(hdc, 100, 230, buf, lstrlenW(buf));
		ReleaseDC(hwnd, hdc);
		return 0;

	case WM_KEYDOWN:
		hdc = GetDC(hwnd);
		wsprintf(buf, L"Virtual keycode: %d ASCII: %c   ", wParam, wParam);
		TextOut(hdc, 100, 200, buf, lstrlenW(buf));
		ReleaseDC(hwnd, hdc);
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		}
		return 0;

	case WM_NCMOUSEMOVE:
	case WM_MOUSEMOVE:
		//SetCursorPos(213, 213);
		//MoveWindow(hwnd, LOWORD(lParam) - 100, HIWORD(lParam) - 10, 400, 400, TRUE);
		hdc = GetDC(hwnd);
		wsprintf(buf, L"x: %d y: %d             ", LOWORD(lParam), HIWORD(lParam));
		TextOut(hdc, 100, 200, buf, lstrlenW(buf));
		ReleaseDC(hwnd, hdc);
		return 0;
	case WM_RBUTTONDOWN:
		hdc = GetDC(hwnd);
		GetClientRect(hwnd, &rect);
		PatBlt(hdc, rect.left, rect.top, rect.right, rect.bottom, WHITENESS);
		ReleaseDC(hwnd, hdc);
		return 0;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}