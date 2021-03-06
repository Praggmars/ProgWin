#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define TIMER1 100

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
	static HDC hmem;
	static HBITMAP hbmp;
	int x, y;
	static int scrollVal = 0;

	switch (iMsg)
	{
	case WM_TIMER:
		switch (wParam)
		{
		case TIMER1:
			hdc = GetDC(hwnd);
			scrollVal = (scrollVal + 10) % GetDeviceCaps(hdc, VERTRES);
			ReleaseDC(hwnd, hdc);
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		}
		return 0;

	case WM_CREATE:
		hdc = GetWindowDC(hwnd);
		hmem = CreateCompatibleDC(hdc);
		hbmp = CreateCompatibleBitmap(hdc, GetDeviceCaps(hdc, HORZRES), 2 * GetDeviceCaps(hdc, VERTRES));
		SelectObject(hmem, hbmp);
		SetBkColor(hmem, 0);
		for (y = 0; y < 2 * GetDeviceCaps(hdc, VERTRES); y += 20)
			for (x = 0; x < GetDeviceCaps(hdc, HORZRES); x += 10)
			{
				WCHAR ch = rand() & 0xff;
				if ((ch >= 'A' && ch <= 'Z') ||
					(ch >= 'a' && ch <= 'z') ||
					(ch >= '0' && ch <= '9'))
				{
					SetTextColor(hmem, RGB(0, (rand() & 0x7f) | 0x80, 0));
					TextOut(hmem, x, y, &ch, 1);
				}
			}
		ReleaseDC(hwnd, hdc);
		SetTimer(hwnd, TIMER1, 20, NULL);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		BitBlt(hdc, 0, 0, rect.right, rect.bottom, hmem, 0, GetDeviceCaps(hdc, VERTRES) - scrollVal, SRCCOPY);
		BitBlt(hdc, 0, 0, rect.right, rect.bottom, hmem, 0, 2 * GetDeviceCaps(hdc, VERTRES) - scrollVal, SRCCOPY);
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		DeleteDC(hmem);
		DeleteObject(hbmp);
		KillTimer(hwnd, TIMER1);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}