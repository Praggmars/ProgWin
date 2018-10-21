#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

const WCHAR appName[] = L"Dance";
HWND g_mainWindow;

COLORREF g_screen[SCREEN_HEIGHT][SCREEN_WIDTH];

typedef struct Frame
{
	POINT *points;
	LONG count;
}Frame;
typedef struct FrameQueue
{
	Frame *frames;
	LONG count;
	LONG activeFrame;
}FrameQueue;

FrameQueue g_frames;

void DrawNextFrame()
{
	LONG i;
	for (i = 0; i < g_frames.frames[g_frames.activeFrame].count; i++)
	{
		POINT p = g_frames.frames[g_frames.activeFrame].points[i];
		g_screen[p.x][p.y + 100] = 0;
	}
	g_frames.activeFrame = (g_frames.activeFrame + 1) % g_frames.count;
	for (i = 0; i < g_frames.frames[g_frames.activeFrame].count; i++)
	{
		POINT p = g_frames.frames[g_frames.activeFrame].points[i];
		g_screen[p.x][p.y + 100] = 0xffffffff;
	}
	InvalidateRect(g_mainWindow, NULL, FALSE);
}

void ReadFramesDAT()
{
	LONG i;
	FILE *infile = fopen("dance.dat", "rb");
	if (infile == NULL)
	{
		PostQuitMessage(0);
		return;
	}
	fread(&g_frames.count, sizeof(LONG), 1, infile);
	g_frames.frames = (Frame*)calloc(g_frames.count, sizeof(Frame));
	if (g_frames.frames == NULL)
	{
		PostQuitMessage(0);
		return;
	}
	for (i = 0; i < g_frames.count; i++)
	{
		fread(&g_frames.frames[i].count, sizeof(LONG), 1, infile);
		g_frames.frames[i].points = (POINT*)calloc(g_frames.frames[i].count, sizeof(POINT));
		if (g_frames.frames[i].points == NULL)
		{
			PostQuitMessage(0);
			return;
		}
		fread(g_frames.frames[i].points, sizeof(POINT), g_frames.frames[i].count, infile);
	}
}
void FreeFramesData()
{
	if (g_frames.frames != NULL)
	{
		int i;
		for (i = 0; i < g_frames.count; i++)
			free(g_frames.frames[i].points);
		free(g_frames.frames);
		ZeroMemory(&g_frames, sizeof(g_frames));
	}
}

VOID CALLBACK TimerFunction(HWND hwnd, UINT uint, UINT_PTR uint_ptr, DWORD dword)
{
	DrawNextFrame();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	HDC hdc;
	HDC hmem;
	HBITMAP hbmp;
	PAINTSTRUCT ps;

	switch (msg)
	{
	case WM_CREATE:
		SetTimer(hwnd, 1, 69, TimerFunction);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		hbmp = CreateBitmap(SCREEN_WIDTH, SCREEN_HEIGHT, 1, 32, g_screen);
		hmem = CreateCompatibleDC(hdc);
		SelectObject(hmem, hbmp);
		BitBlt(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, hmem, 0, 0, SRCCOPY);
		DeleteObject(hmem);

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
	WNDCLASS wc;
	DWORD style, styleEx;;
	RECT rect;
	MSG msg;

	ZeroMemory(&g_frames, sizeof(g_frames));
	ReadFramesDAT();

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
	rect.right = SCREEN_WIDTH;
	rect.bottom = SCREEN_HEIGHT;
	style = WS_OVERLAPPEDWINDOW & (~(WS_THICKFRAME | WS_MAXIMIZEBOX));
	styleEx = WS_EX_OVERLAPPEDWINDOW;
	AdjustWindowRectEx(&rect, style, FALSE, styleEx);
	g_mainWindow = CreateWindowEx(styleEx, appName, appName, style,
		(GetSystemMetrics(SM_CXSCREEN) - rect.right + rect.left) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - rect.bottom + rect.top) / 2,
		rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);

	ShowWindow(g_mainWindow, iCmdShow);
	UpdateWindow(g_mainWindow);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	FreeFramesData();

	return msg.wParam;
}