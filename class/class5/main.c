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
	HBITMAP hbmp;
	BITMAP bmp;
	HDC hmem;

	switch (iMsg)
	{
	case WM_CREATE:
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);

		hmem = CreateCompatibleDC(hdc);
		/*hbmp = CreateCompatibleBitmap(hdc, 2000, 2000);
		SelectObject(hmem, hbmp);
		PatBlt(hmem, 0, 0, 2000, 2000, WHITENESS);
		Rectangle(hmem, 100, 100, 1800, 1800);
		TextOut(hmem, 213, 213, TEXT("213"), 3);
		//BitBlt(hdc, 100, 100, 500, 500, hmem, 0, 0, SRCCOPY);
		StretchBlt(hdc, 100, 100, 500, 500, hmem, 0, 0, 2000, 2000, SRCCOPY);*/
		/*hbmp = LoadImage(0, L"image2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		GetObject(hbmp, sizeof(bmp), &bmp);
		SelectObject(hmem, hbmp);
		StretchBlt(hdc, 0, 0, rect.right, rect.bottom, hmem, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);*/
		hbmp = CreateCompatibleBitmap(hdc, rect.right / 10, rect.bottom / 10);
		SelectObject(hmem, hbmp);
		TextOut(hmem, 20, 30, TEXT("213"), 3);
		StretchBlt(hdc, 0, 0, rect.right, rect.bottom, hmem, 0, 0, rect.right / 10, rect.bottom / 10, SRCCOPY);
		DeleteDC(hmem);
		DeleteObject(hbmp);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}