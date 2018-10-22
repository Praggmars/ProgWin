#include "danceapp.h"
#include "fractalapp.h"
#include "multableapp.h"
#include "sandpileapp.h"
#include "spheresapp.h"
#include "function.h"
#include "gameoflife.h"
#include "mandelbrotapp.h"
#include "juliaapp.h"
#include <ctime>

#define ID_DANCEAPP 101
#define ID_FRACTALAPP 102
#define ID_MULTABLEAPP 103
#define ID_SANDPILEAPP 104
#define ID_SPHERESAPP 105
#define ID_FUNCTIONAPP 106
#define ID_GAMEOFLIFEAPP 107
#define ID_MANDELBROTAPP 108
#define ID_JULIAAPP 109

AppBase *g_app;

void SafeDeleteApp()
{
	if (g_app)
	{
		delete g_app;
		g_app = nullptr;
	}
}

void StartApp(HWND hwnd, int id)
{
	switch (id)
	{
	case ID_DANCEAPP:
		SafeDeleteApp();
		g_app = new dnc::DanceApp(hwnd);
		break;
	case ID_FRACTALAPP:
		SafeDeleteApp();
		g_app = new frac::FractalApp(hwnd);
		break;
	case ID_MULTABLEAPP:
		SafeDeleteApp();
		g_app = new mtbl::MultableApp(hwnd);
		break;
	case ID_SANDPILEAPP:
		SafeDeleteApp();
		g_app = new spl::SandPileApp(hwnd);
		break;
	case ID_SPHERESAPP:
		SafeDeleteApp();
		g_app = new sph::SpheresApp(hwnd);
		break;
	case ID_FUNCTIONAPP:
		SafeDeleteApp();
		g_app = new fnc::FunctionApp(hwnd);
		break;
	case ID_GAMEOFLIFEAPP:
		SafeDeleteApp();
		g_app = new gof::GameOfLifeApp(hwnd);
		break;
	case ID_MANDELBROTAPP:
		SafeDeleteApp();
		g_app = new mdb::MandelbrotApp(hwnd);
		break;
	case ID_JULIAAPP:
		SafeDeleteApp();
		g_app = new jul::JuliaApp(hwnd);
		break;
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (g_app)
		g_app->MessageHandler(hwnd, msg, wparam, lparam);
	switch (msg)
	{
	case WM_COMMAND:
		StartApp(hwnd, LOWORD(wparam));
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR szCMLine, int iCmdShow)
{
	WNDCLASS wc;
	CONST WCHAR title[] = L"AllApp";
	MSG msg;
	RECT rect;

	srand((unsigned int)time(NULL));

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = title;
	RegisterClass(&wc);

	HMENU hmenu = CreateMenu();
	AppendMenu(hmenu, MF_STRING, ID_DANCEAPP, L"Dance");
	AppendMenu(hmenu, MF_STRING, ID_FRACTALAPP, L"Fractal");
	AppendMenu(hmenu, MF_STRING, ID_MULTABLEAPP, L"MulTable");
	AppendMenu(hmenu, MF_STRING, ID_SANDPILEAPP, L"Sandpile");
	AppendMenu(hmenu, MF_STRING, ID_SPHERESAPP, L"Shperes");
	AppendMenu(hmenu, MF_STRING, ID_FUNCTIONAPP, L"Function");
	AppendMenu(hmenu, MF_STRING, ID_GAMEOFLIFEAPP, L"GameOfLife");
	AppendMenu(hmenu, MF_STRING, ID_MANDELBROTAPP, L"Mandelbrot");
	AppendMenu(hmenu, MF_STRING, ID_JULIAAPP, L"Julia");

	g_app = nullptr;
	rect.left = 0;
	rect.right = 640;
	rect.top = 0;
	rect.bottom = 480;
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, TRUE, WS_EX_OVERLAPPEDWINDOW);
	HWND hwnd = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW, title, title, WS_OVERLAPPEDWINDOW,
		(GetSystemMetrics(SM_CXSCREEN) - rect.right + rect.left) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - rect.bottom + rect.top) / 2,
		rect.right - rect.left, rect.bottom - rect.top, NULL, hmenu, hInstance, NULL);

	GetClientRect(hwnd, &rect);
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	SafeDeleteApp();

	return msg.wParam;
}