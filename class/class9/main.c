#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include "resource.h"

#define LISTBOX1 200
#define STATIC1 201
#define RADIO1 202
#define RADIO2 203
#define RADIO3 204
#define RADIO4 205


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR szCmdLine, int iCmdShow)
{
	static WCHAR szAppName[] = L"WinApp";
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

	hwnd = CreateWindow(szAppName, L"Windows App", WS_OVERLAPPEDWINDOW,
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

	switch (iMsg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			HDC hdc;
			RECT rect;
		case ID_HELP:
			MessageBox(hwnd, TEXT("HELP!!!"), L"Title", MB_OK | MB_ICONINFORMATION);
			break;
		case ID_SHAPE_RECTANGLE:
			hdc = GetDC(GetDlgItem(hwnd, STATIC1));
			Rectangle(hdc, 10, 10, 90, 150);
			ReleaseDC(hwnd, hdc);
			SendMessage(GetDlgItem(hwnd, LISTBOX1), LB_ADDSTRING, 0, (LPARAM)L"Rectangle");
			break;
		case ID_SHAPE_ELLIPSE:
			hdc = GetDC(GetDlgItem(hwnd, STATIC1));
			Ellipse(hdc, 10, 10, 90, 150);
			ReleaseDC(hwnd, hdc);
			SendMessage(GetDlgItem(hwnd, LISTBOX1), LB_ADDSTRING, 0, (LPARAM)L"Ellipse");
			break;
		case ID_CLEAR:
			hdc = GetDC(GetDlgItem(hwnd, STATIC1));
			GetClientRect(GetDlgItem(hwnd, STATIC1), &rect);
			PatBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, WHITENESS);
			ReleaseDC(hwnd, hdc);
			SendMessage(GetDlgItem(hwnd, LISTBOX1), LB_RESETCONTENT, 0, 0);
			break;
		}
		return 0;

	case WM_CREATE:
		CreateWindow(L"listbox", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL, 10, 10, 100, 194, hwnd, (HMENU)LISTBOX1, NULL, NULL);
		CreateWindow(L"static", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 120, 10, 100, 194, hwnd, (HMENU)STATIC1, NULL, NULL);
		CreateWindow(L"button", L"radio1", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 230, 10, 100, 20, hwnd, (HMENU)RADIO1, NULL, NULL);
		CreateWindow(L"button", L"radio2", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 230, 40, 100, 20, hwnd, (HMENU)RADIO2, NULL, NULL);
		CreateWindow(L"button", L"radio3", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 230, 70, 100, 20, hwnd, (HMENU)RADIO3, NULL, NULL);
		CreateWindow(L"button", L"radio4", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 230, 100, 100, 20, hwnd, (HMENU)RADIO4, NULL, NULL);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}