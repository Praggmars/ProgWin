#include <windows.h>

const WCHAR appName[] = L"App1";
const WCHAR app2[] = L"App2"; 
HWND g_mainWindow;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static unsigned char color = 0;
	HDC hdc;
	PAINTSTRUCT ps;

	switch (msg)
	{
	case WM_COMMAND:
		if (wparam == 1)
			CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, app2, app2, 
				WS_OVERLAPPEDWINDOW | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 
				100, 100, 400, 400, hwnd, NULL, GetModuleHandle(NULL), NULL);
		return 0;
	case WM_CREATE:
		CreateWindow(L"button", L"Add Window", WS_VISIBLE | WS_CHILD, 10, 10, 100, 24, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);
		return 0;	
	case WM_PAINT:
		color += 1;
		hdc = BeginPaint(hwnd, &ps);
		SetTextColor(hdc, RGB(color, 0, color));
		TextOut(hdc, 40, 40, L"Drawn", 5);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		if (g_mainWindow == hwnd)
			PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR szCMLine, int iCmdShow)
{
	MSG msg;
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = appName;
	RegisterClass(&wc);

	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszClassName = app2;
	RegisterClass(&wc);


	g_mainWindow = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, appName, appName, WS_OVERLAPPEDWINDOW & (~(WS_THICKFRAME | WS_MAXIMIZEBOX)),
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ShowWindow(g_mainWindow, iCmdShow);
	//UpdateWindow(hwnd1);

	//ShowWindow(hwnd2, iCmdShow);
	//UpdateWindow(hwnd2);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}