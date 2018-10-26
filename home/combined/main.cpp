#include "danceapp.h"
#include "fractalapp.h"
#include "multableapp.h"
#include "sandpileapp.h"
#include "spheresapp.h"
#include "function.h"
#include "gameoflife.h"
#include "mandelbrotapp.h"
#include "juliaapp.h"
#include "newtonapp.h"
#include "rippleapp.h"
#include <ctime>

#define ID_HELP 100
#define ID_DANCEAPP 101
#define ID_FRACTALAPP 102
#define ID_MULTABLEAPP 103
#define ID_SANDPILEAPP 104
#define ID_SPHERESAPP 105
#define ID_FUNCTIONAPP 106
#define ID_GAMEOFLIFEAPP 107
#define ID_MANDELBROTAPP 108
#define ID_JULIAAPP 109
#define ID_NEWTONAPP 110
#define ID_RIPPLEAPP 111

AppBase *g_app;

void SafeDeleteApp()
{
	if (g_app)
	{
		delete g_app;
		g_app = nullptr;
	}
}

void ShowHelpDialog(HWND hwnd)
{
	RECT rect;
	GetWindowRect(hwnd, &rect);
	HWND dialogWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"DialogBox", L"Help",
		WS_OVERLAPPEDWINDOW&~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME),
		rect.left + 50, rect.top + 50, 400, 150, hwnd, NULL, GetModuleHandle(NULL), NULL);
	ShowWindow(dialogWnd, SW_SHOW);
	MessageBeep(MB_OK);
}

void StartApp(HWND hwnd, int id)
{
	switch (id)
	{
	case ID_HELP:
		ShowHelpDialog(hwnd);
		break;
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
	case ID_NEWTONAPP:
		SafeDeleteApp();
		g_app = new ntn::NewtonApp(hwnd);
		break;
	case ID_RIPPLEAPP:
		SafeDeleteApp();
		g_app = new rpl::RippleApp(hwnd);
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

void RegisterWindow(HINSTANCE hInstance)
{
	WNDCLASS wc = {};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"CombinedApp";
	RegisterClass(&wc);
}

LRESULT CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;
	switch (msg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		rect.top += 10;
		DrawText(hdc, g_app ? g_app->HelpDialogText() : L"Select a program to run", -1, &rect, DT_CENTER | DT_VCENTER);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}
void RegisterDialogBox(HINSTANCE hInstance)
{
	WNDCLASS wc = {};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = DlgProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = L"DialogBox";
	RegisterClass(&wc);
}

HMENU CreateAppMenu()
{
	HMENU hmenu = CreateMenu();
	HMENU programMenu = CreateMenu();
	AppendMenu(programMenu, MF_STRING, ID_DANCEAPP, L"Dance");
	AppendMenu(programMenu, MF_STRING, ID_FRACTALAPP, L"Fractal");
	AppendMenu(programMenu, MF_STRING, ID_MULTABLEAPP, L"MulTable");
	AppendMenu(programMenu, MF_STRING, ID_SANDPILEAPP, L"Sandpile");
	AppendMenu(programMenu, MF_STRING, ID_SPHERESAPP, L"Shperes");
	AppendMenu(programMenu, MF_STRING, ID_FUNCTIONAPP, L"Function");
	AppendMenu(programMenu, MF_STRING, ID_GAMEOFLIFEAPP, L"GameOfLife");
	AppendMenu(programMenu, MF_STRING, ID_RIPPLEAPP, L"Ripple");
	AppendMenu(programMenu, MF_STRING, ID_MANDELBROTAPP, L"Mandelbrot");
	AppendMenu(programMenu, MF_STRING, ID_JULIAAPP, L"Julia");
	AppendMenu(programMenu, MF_STRING, ID_NEWTONAPP, L"Newton");
	AppendMenu(hmenu, MF_POPUP, (UINT_PTR)programMenu, L"Programs");
	AppendMenu(hmenu, MF_STRING, ID_HELP, L"Help");
	return hmenu;
}
void CreateAppWindow(HINSTANCE hInstance, int iCmdShow)
{
	RECT rect;
	rect.left = 0;
	rect.right = 640;
	rect.top = 0;
	rect.bottom = 480;
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, TRUE, WS_EX_OVERLAPPEDWINDOW);
	HWND hwnd = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW, L"CombinedApp", L"AllApp", WS_OVERLAPPEDWINDOW,
		(GetSystemMetrics(SM_CXSCREEN) - rect.right + rect.left) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - rect.bottom + rect.top) / 2,
		rect.right - rect.left, rect.bottom - rect.top, NULL,
		CreateAppMenu(), hInstance, NULL);
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR szCMLine, int iCmdShow)
{
	srand((unsigned int)time(NULL));
	g_app = nullptr;

	RegisterWindow(hInstance);
	RegisterDialogBox(hInstance);
	CreateAppWindow(hInstance, iCmdShow);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	SafeDeleteApp();

	return msg.wParam;
}