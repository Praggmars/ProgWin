/*------------------------------------------------------------------
   APIHello.C -- skeleton for a simple API-based "HELLO" application
  ------------------------------------------------------------------*/

#define _CRT_SECURE_NO_WARNINGS		// Remove secure warnings 

#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
	
	hwnd = CreateWindow(szAppName,
		L"The Hello Program",
		WS_OVERLAPPEDWINDOW | WS_VSCROLL,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

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
	static unsigned char color = 0;

	static int scrollY = 0;

	switch (iMsg)
	{
	case WM_VSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_LINEUP:
			scrollY--;
			break;
		case SB_LINEDOWN:
			scrollY++;
			break;
		case SB_PAGEUP:
			scrollY -= 10;
			break;
		case SB_PAGEDOWN:
			scrollY += 10;
			break;
		}
		if (scrollY > 100)scrollY = 100;
		if (scrollY < 0)scrollY = 0;
		SetScrollPos(hwnd, SB_VERT, scrollY, TRUE);
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
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
		// Initialization
		SetScrollRange(hwnd, SB_VERT, 0, 100, TRUE);
		CreateWindow(L"button", L"Quit", WS_VISIBLE | WS_CHILD, 670, 200, 200, 30, hwnd, (HMENU)1, NULL, NULL);
		CreateWindow(L"button", L"Sound", WS_VISIBLE | WS_CHILD, 670, 240, 200, 30, hwnd, (HMENU)2, NULL, NULL);
		return 0;

	case WM_PAINT:
		color += 10;
		hdc = BeginPaint(hwnd, &ps); // Get DC handle
		TEXTMETRIC tm;

		GetTextMetrics(hdc, &tm);
		GetClientRect(hwnd, &rect);  // Get client rectangle

		SetTextColor(hdc, RGB(color, 0, color));
		for (int y = 1; y <= 1000; y++)
			for (int x = 1; x <= 10; x++)
			{
				char str[10];
				WCHAR str2[10];
				int i;
				sprintf(str, "%d", x*y);
				for (i = 0; str[i]; i++)
					str2[i] = str[i];
				TextOut(hdc, x*tm.tmMaxCharWidth * 4, y*tm.tmHeight - scrollY * rect.bottom / 10, str2, i);
			}

		EndPaint(hwnd, &ps);         // Release DC
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);          // Exit from the program
		return 0;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam); // Call the default window procedure
}