#include <windows.h>
#include <fstream>
#include <vector>

const WCHAR appName[] = L"Dance";
int g_width, g_height;
int g_frameTime;
HWND g_mainWindow;

std::vector<HBITMAP> g_frames;
size_t g_currentFrame = 0;

bool LoadFrames()
{
	std::ifstream infile;
	infile.open(L"dance.big", std::ios::in | std::ios::binary);
	if (!infile.good())
		return false;
	int frameCount;
	infile.read((char*)&frameCount, sizeof(int));
	infile.read((char*)&g_frameTime, sizeof(int));
	infile.read((char*)&g_width, sizeof(int));
	infile.read((char*)&g_height, sizeof(int));
	std::vector<COLORREF> pixels(g_width*g_height);
	for (int i = 0; i < frameCount; i++)
	{
		infile.read((char*)pixels.data(), sizeof(pixels[0])*pixels.size());
		if (!infile.good())
			return false;
		HBITMAP hbmp = CreateBitmap(g_width, g_height, 1, 32, pixels.data());
		if (hbmp == NULL)
			return false;
		g_frames.push_back(hbmp);
	}
	infile.close();
	return true;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	HDC hdc;
	HDC hmem;
	HBITMAP hbmp;
	PAINTSTRUCT ps;
	RECT rect;

	switch (msg)
	{
	case WM_CREATE:
		SetTimer(hwnd, 1, g_frameTime, NULL);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);

		hbmp = g_frames[g_currentFrame];
		hmem = CreateCompatibleDC(hdc);
		SelectObject(hmem, hbmp);
		//BitBlt(hdc, 0, 0, g_width, g_height, hmem, 0, 0, SRCCOPY);
		if (rect.right * g_height > rect.bottom * g_width)
		{
			int offset = (rect.right - (g_width * rect.bottom / g_height)) / 2;
			rect.left += offset;
			rect.right -= offset;
		}
		else
		{
			int offset = (rect.bottom - g_height * rect.right / g_width) / 2;
			rect.top += offset;
			rect.bottom -= offset;
		}
		StretchBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, hmem, 0, 0, g_width, g_height, SRCCOPY);
		DeleteObject(hmem);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_TIMER:
		if (wparam == 1)
		{
			g_currentFrame = (g_currentFrame + 1) % g_frames.size();
			InvalidateRect(hwnd, NULL, FALSE);
		}
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
	WNDCLASS wc;
	RECT rect;
	MSG msg;

	if (!LoadFrames())
		return 0;

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

	rect.left = 0;
	rect.top = 0;
	rect.right = g_width;
	rect.bottom = g_height;
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW);
	g_mainWindow = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, appName, appName, WS_OVERLAPPEDWINDOW,
		max((GetSystemMetrics(SM_CXSCREEN) - rect.right + rect.left) / 2, 0),
		max((GetSystemMetrics(SM_CYSCREEN) - rect.bottom + rect.top) / 2, 0),
		rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);

	ShowWindow(g_mainWindow, iCmdShow);
	UpdateWindow(g_mainWindow);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}