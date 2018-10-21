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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC         hdc;
	PAINTSTRUCT ps;
	RECT        rect;
	POINT p;
	HPEN hp;
	int n;
	POINT *s;

	static unsigned char color = 0;

	static int scrollY = 0;

	switch (iMsg)
	{
	case WM_COMMAND:
		switch (wParam)
		{
		case 1:
			if (IDYES == MessageBox(hwnd, L"Sure?", L"Quit", MB_YESNO))
				PostQuitMessage(0);
			return 0;
		case 2:
			MessageBeep(MB_OK);
			return 0;
		}
		return 0;
	case WM_CREATE:
		CreateWindow(L"button", L"Quit", WS_VISIBLE | WS_CHILD, 670, 200, 200, 30, hwnd, (HMENU)1, NULL, NULL);
		CreateWindow(L"button", L"Sound", WS_VISIBLE | WS_CHILD, 670, 240, 200, 30, hwnd, (HMENU)2, NULL, NULL);
		return 0;

	case WM_PAINT:
		color += 1;
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		hp = CreatePen(PS_SOLID, 20, RGB(color / 2, color / 2, 0));
		SelectObject(hdc, hp);

		LineTo(hdc, rect.right, rect.bottom);
		MoveToEx(hdc, rect.right, 0, &p);
		LineTo(hdc, 0, rect.bottom);

		Rectangle(hdc, rect.right / 3, rect.bottom / 3, 2 * rect.right / 3, 2 * rect.bottom / 3);
		s = (POINT*)malloc(sizeof(POINT)*rect.right / 3);
		for (n = 0; n < rect.right / 3; n++)
		{
			s[n].x = n + rect.right / 3;
			s[n].y = (-(LONG)sin(n * 3.141593 * 2 / rect.right * 3) + 1) * rect.bottom / 6 + rect.bottom / 3;
		}
		Polyline(hdc, s, rect.right / 3);
		free(s);

		DeleteObject(hp);
		EndPaint(hwnd, &ps);
		return 0;

	case WM_LBUTTONDOWN:
		hdc = GetDC(hwnd);
		//hdc = GetWindowDC(NULL);
		//hdc = GetWindowDC(hwnd);
		hp = CreatePen(PS_SOLID, 50, RGB(color, color, 0));
		SelectObject(hdc, hp);
		Ellipse(hdc, 40, 200, 500, 400);
		DeleteObject(hp);
		TextOut(hdc, 0, 0, L"TEXT", 4);
		ReleaseDC(hwnd, hdc);
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