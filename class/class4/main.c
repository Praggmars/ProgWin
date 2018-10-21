#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PWSTR szCmdLine, int iCmdShow)
{
	static WCHAR szAppName[] = L"HelloWin";
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
		szAppName, L"The Hello Program", WS_OVERLAPPEDWINDOW,
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

void Part1(HDC hdc, RECT rect)
{
	SetMapMode(hdc, MM_ISOTROPIC);
	SetViewportExtEx(hdc, rect.right, -rect.bottom, NULL);
	SetWindowExtEx(hdc, 30000, 30000, NULL);
	SetViewportOrgEx(hdc, 0, rect.bottom, NULL);
	MoveToEx(hdc, 0, 0, NULL);
	LineTo(hdc, 15000, 15000);
	TextOut(hdc, 20000, 20000, L"wahetrev", 9);
}
void Part2(HDC hdc, RECT rect)
{
	TEXTMETRIC tm;
	SetMapMode(hdc, MM_ANISOTROPIC);
	SetWindowExtEx(hdc, 1, 1, NULL);
	GetTextMetrics(hdc, &tm);
	SetViewportExtEx(hdc, tm.tmAveCharWidth, tm.tmHeight + tm.tmExternalLeading, NULL);
	TextOut(hdc, 0, 0, TEXT("hello"), 5);
	TextOut(hdc, 1, 1, TEXT("hello"), 5);
	TextOut(hdc, 2, 2, TEXT("hello"), 5);
}
void Part3(HDC hdc, HWND hwnd, RECT rect)
{
	HRGN hr1, hr2, hr3;
	HBRUSH hb;
	int n;
	//hb = CreateHatchBrush(0, RGB(0x3f, 0xa2, 0xa0));
	hb = CreateSolidBrush(RGB(0x3f, 0xa2, 0xa0));
	SelectObject(hdc, hb);
	hr1 = CreateEllipticRgn(100, 100, 400, 500);
	hr2 = CreateEllipticRgn(300, 100, 600, 500);
	hr3 = CreateEllipticRgn(0, 0, 0, 0);
	CombineRgn(hr3, hr1, hr2, RGN_OR);

	SelectClipRgn(hdc, hr3);
	SetWindowRgn(hwnd, hr3, FALSE);
	for (n = 0; n < rect.bottom; n += 10)
	{
		MoveToEx(hdc, 0, n, NULL);
		LineTo(hdc, rect.right, n);
	}

	FrameRgn(hdc, hr3, hb, 20, 15);
	DeleteObject(hb);
	DeleteObject(hr1);
	DeleteObject(hr2);
	DeleteObject(hr3);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	HRGN hr;

	switch (iMsg)
	{
	case WM_CREATE:
		GetClientRect(hwnd, &rect);
		hr = CreateEllipticRgn(0, 0, rect.right, rect.bottom);
		SetWindowRgn(hwnd, hr, TRUE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);

		//Part1(hdc, rect);
		//Part2(hdc, rect);
		Part3(hdc, hwnd, rect);

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