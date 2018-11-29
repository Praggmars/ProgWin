#define _CRT_SECURE_NO_WARNINGS

#include "resource.h"
#include <windows.h>

typedef struct
{
	POINT mousePosition;
	RECT rect;
	LPWSTR fontName;
}AppData;

AppData g_appData;

void InitAppData()
{
	g_appData.mousePosition.x = 0;
	g_appData.mousePosition.y = 0;
	g_appData.rect.left = 0;
	g_appData.rect.top = 0;
	g_appData.rect.right = 0;
	g_appData.rect.bottom = 0;
	g_appData.fontName = L"Arial";
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR szCmdLine, int iCmdShow)
{
	static WCHAR szAppName[] = L"ExamApp";
	HWND hwnd;
	MSG msg;
	WNDCLASSEX wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wndclass.lpszClassName = szAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(szAppName, L"Nov15Exam", WS_OVERLAPPEDWINDOW,
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

void Paint200x200Square(HDC hdc)
{
	int w = g_appData.rect.right / 2;
	int h = g_appData.rect.bottom * 3 / 4;
	int mx = g_appData.mousePosition.x;
	int my = g_appData.mousePosition.y;
	MoveToEx(hdc, w - 101, h - 101, NULL);
	LineTo(hdc, w + 100, h - 101);
	LineTo(hdc, w + 100, h + 100);
	LineTo(hdc, w - 101, h + 100);
	LineTo(hdc, w - 101, h - 101);
	StretchBlt(hdc, w - 100, h - 100, 200, 200, hdc, mx - 50, my - 50, 100, 100, SRCCOPY);
}

void DrawCharacter(HWND hwnd, WCHAR ch)
{
	if (g_appData.mousePosition.y + 30 < g_appData.rect.bottom / 2)
	{
		HFONT hf;
		LOGFONT lf;
		HDC hdc = GetDC(hwnd);
		lf.lfHeight = 60;
		lf.lfWidth = 30;
		lf.lfEscapement = 0;
		lf.lfUnderline = FALSE;
		lf.lfStrikeOut = FALSE;
		lf.lfItalic = FALSE;
		lf.lfWeight = 0x7fffffff;
		lstrcpyW(lf.lfFaceName, g_appData.fontName);
		hf = CreateFontIndirect(&lf);
		SelectObject(hdc, hf);
		SetTextColor(hdc, RGB(0, 0, 0));
		TextOut(hdc, g_appData.mousePosition.x - 15, g_appData.mousePosition.y - 30, &ch, 1);
		DeleteObject(hf);
		Paint200x200Square(hdc);
		ReleaseDC(hwnd, hdc);
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	GetClientRect(hwnd, &g_appData.rect);

	switch (iMsg)
	{
	case WM_CREATE:
		InitAppData();
		return 0;

	case WM_COMMAND:
		switch (wParam)
		{
		case ID_CLEAR:
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case ID_CHARACTER_ARIAL:
			g_appData.fontName = L"Arial";
			break;
		case ID_CHARACTER_TIMES:
			g_appData.fontName = L"Times";
			break;
		case ID_CHARACTER_COURIER:
			g_appData.fontName = L"Courier";
			break;
		}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		MoveToEx(hdc, 0, g_appData.rect.bottom / 2, NULL);
		LineTo(hdc, g_appData.rect.right, g_appData.rect.bottom / 2);
		Paint200x200Square(hdc);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_CHAR:
		DrawCharacter(hwnd, (WCHAR)wParam);
		return 0;

	case WM_MOUSEMOVE:
		g_appData.mousePosition.x = LOWORD(lParam);
		g_appData.mousePosition.y = HIWORD(lParam);
		hdc = GetDC(hwnd);
		Paint200x200Square(hdc);
		ReleaseDC(hwnd, hdc);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}