#include "strf_stringfunction.h"
#include <windows.h>


#define ID_FUNCTION_INPUT 1
#define ID_RESET_BUTTON 2

std::wstring g_functionStr(L"1/(1-exp(1/sin(x)))");
//std::wstring g_functionStr(L"abs((1+(x+1)*(x-2))*(sqrt(x)-0.5))");

strf::Function g_strFunc;

HWND g_mainWindow;
HWND g_functionInput;
HWND g_resetButton;
HWND g_xPosText;
HWND g_yPosText;
HWND g_fxPosText;

struct GraphSettings
{
	int mx;
	int my;
	double distanceFromMiddle;
	double prevwixau;

	double width;
	double height;
	double middlex;
	double middley;
	double widthInXAxisUnit;

	inline void DefaultSettings()
	{
		mx = 0;
		my = 0;
		middlex = 0.0;
		middley = 0.0;
		widthInXAxisUnit = 4.0;
	}
	inline double Zoom()
	{
		return width / widthInXAxisUnit;
	}
	inline double ToGraphXCoord(int x)
	{
		return (x - width / 2) / Zoom() + middlex;
	}
	inline double ToGraphYCoord(int y)
	{
		return (height / 2 - y) / Zoom() + middley;
	}
	inline int ToScreenXCoordClamp(double x)
	{
		x = (x - middlex) * Zoom() + width / 2;
		if (x > INT16_MAX)
			return INT16_MAX;
		if (x < INT16_MIN)
			return INT16_MIN;
		return (int)x;
	}
	inline int ToScreenYCoordClamp(double y)
	{
		y = (middley - y) * Zoom() + height / 2;
		if (y > INT16_MAX)
			return INT16_MAX;
		if (y < INT16_MIN)
			return INT16_MIN;
		return (int)y;
	}
	inline int ToScreenXCoord(double x)
	{
		return (int)((x - middlex) * Zoom() + width / 2);
	}
	inline int ToScreenYCoord(double y)
	{
		return (int)((middley - y) * Zoom() + height / 2);
	}
};

GraphSettings g_graphSettings;


void FillCoordTextBoxes()
{
	SetWindowText(g_xPosText, (L"x = " + std::to_wstring(g_graphSettings.ToGraphXCoord(g_graphSettings.mx))).c_str());
	SetWindowText(g_yPosText, (L"y = " + std::to_wstring(g_graphSettings.ToGraphYCoord(g_graphSettings.my))).c_str());
	SetWindowText(g_fxPosText, (L"f(x) = " + std::to_wstring(g_strFunc(g_graphSettings.ToGraphXCoord(g_graphSettings.mx)))).c_str());
}

void RedrawRequest()
{
	InvalidateRect(g_mainWindow, NULL, FALSE);
}

void DrawFunction(HDC hdc, std::function<double(double)> f)
{
	bool valid = false;
	double x, y;
	int draw_x, draw_y;
	HPEN hpen = CreatePen(PS_SOLID, 5, 0x1f7fff);
	SelectObject(hdc, hpen);

	for (int i = 2; i < g_graphSettings.width - 2; i++)
	{
		x = g_graphSettings.ToGraphXCoord(i);
		y = f(x);
		if (isnan(y) || isinf(y))
		{
			valid = false;
		}
		else
		{
			draw_x = g_graphSettings.ToScreenXCoordClamp(x);
			draw_y = g_graphSettings.ToScreenYCoordClamp(y);
			if (!valid)
			{
				MoveToEx(hdc, draw_x, draw_y, NULL);
				valid = true;
			}
			else
			{
				LineTo(hdc, draw_x, draw_y);
			}
		}
	}
	DeleteObject(hpen);
}
void DrawAxises(HDC hdc)
{
	HPEN thin = CreatePen(PS_SOLID, 1, 0xffffff);
	HPEN thick = CreatePen(PS_SOLID, 3, 0xffffff);
	SelectObject(hdc, thin);

	int draw_coord;
	int guard;
	int maxLines = 1000;
	double linedist = pow(2.0, floor(log2(g_graphSettings.widthInXAxisUnit))) / 8.0;

	double start = ceil(-g_graphSettings.ToScreenXCoord(0.0) / g_graphSettings.Zoom() / linedist)*linedist;
	double end = ceil((g_graphSettings.width - g_graphSettings.ToScreenXCoord(0.0)) / g_graphSettings.Zoom() / linedist)*linedist;
	guard = 0;
	for (double x = start; x < end; x += linedist)
	{
		draw_coord = (int)(x * g_graphSettings.Zoom() + g_graphSettings.ToScreenXCoord(0.0));
		MoveToEx(hdc, draw_coord, 0, NULL);
		LineTo(hdc, draw_coord, (int)g_graphSettings.height);
		if (++guard > maxLines)
			break;
	}
	start = ceil(-g_graphSettings.ToScreenYCoord(0.0) / g_graphSettings.Zoom() / linedist)*linedist;
	end = ceil((g_graphSettings.height - g_graphSettings.ToScreenYCoord(0.0)) / g_graphSettings.Zoom() / linedist)*linedist;
	guard = 0;
	for (double y = start; y < end; y += linedist)
	{
		draw_coord = (int)(y * g_graphSettings.Zoom() + g_graphSettings.ToScreenYCoord(0.0));
		MoveToEx(hdc, 0, draw_coord, NULL);
		LineTo(hdc, (int)g_graphSettings.width, draw_coord);
		if (++guard > maxLines)
			break;
	}

	SelectObject(hdc, thick);
	if (g_graphSettings.ToScreenYCoord(0.0) < g_graphSettings.height && g_graphSettings.ToScreenYCoord(0.0) >= 0)
	{
		MoveToEx(hdc, 0, g_graphSettings.ToScreenYCoord(0.0), NULL);
		LineTo(hdc, (int)g_graphSettings.width - 2, g_graphSettings.ToScreenYCoord(0.0));
	}
	if (g_graphSettings.ToScreenXCoord(0.0) < g_graphSettings.width && g_graphSettings.ToScreenXCoord(0.0) >= 0)
	{
		MoveToEx(hdc, g_graphSettings.ToScreenXCoord(0.0), 0, NULL);
		LineTo(hdc, g_graphSettings.ToScreenXCoord(0.0), (int)g_graphSettings.height);
	}

	DeleteObject(thin);
	DeleteObject(thick);
}

void LoadFunction()
{
	WCHAR str[512];
	GetWindowText(g_functionInput, str, 511);
	g_strFunc.LoadFunction(str);
	RedrawRequest();
}

void Ev_Command(HWND hwnd, WPARAM wparam)
{
	if (LOWORD(wparam) == ID_FUNCTION_INPUT && HIWORD(wparam) == EN_CHANGE)
		LoadFunction();
	if (LOWORD(wparam) == ID_RESET_BUTTON)
	{
		g_graphSettings.DefaultSettings();
		RedrawRequest();
	}
}
void Ev_Create(HWND hwnd)
{
	g_graphSettings.DefaultSettings();
}
void Ev_Paint(HWND hwnd)
{
	PAINTSTRUCT ps;
	RECT rect;
	GetClientRect(hwnd, &rect);
	rect.right = max(rect.right - 240, 1);
	g_graphSettings.width = max(rect.right, 1);
	g_graphSettings.height = max(rect.bottom, 1);

	HDC hdc = BeginPaint(hwnd, &ps);
	HBRUSH brush = CreateSolidBrush(0x1f1f1f);
	FillRect(hdc, &rect, brush);
	DrawAxises(hdc);
	DrawFunction(hdc, [](double x)->double {return g_strFunc(x); });
	DeleteObject(brush);
	EndPaint(hwnd, &ps);

	MoveWindow(g_xPosText, (int)g_graphSettings.width + 10, 40, 220, 22, TRUE);
	MoveWindow(g_yPosText, (int)g_graphSettings.width + 10, 64, 220, 22, TRUE);
	MoveWindow(g_fxPosText, (int)g_graphSettings.width + 10, 88, 220, 22, TRUE);
	MoveWindow(g_functionInput, (int)g_graphSettings.width + 10, 130, 220, 24, TRUE);
	MoveWindow(g_resetButton, (int)g_graphSettings.width + 10, 160, 220, 24, TRUE);
}
void Ev_MouseWheel(HWND hwnd, WPARAM wparam)
{
	if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
		g_graphSettings.widthInXAxisUnit /= 1.1;
	else
		g_graphSettings.widthInXAxisUnit *= 1.1;
	RedrawRequest();
	FillCoordTextBoxes();
}
void Ev_MMouseButton(LPARAM lparam)
{
	int dx = LOWORD(lparam) - (int)(g_graphSettings.width / 2.0);
	int dy = HIWORD(lparam) - (int)(g_graphSettings.height / 2.0);
	g_graphSettings.distanceFromMiddle = sqrt(dx*dx + dy * dy);
	g_graphSettings.prevwixau = g_graphSettings.widthInXAxisUnit;
}
void Ev_RMouseButton(HWND hwnd, LPARAM lparam)
{
	g_graphSettings.middlex = g_graphSettings.ToGraphXCoord(LOWORD(lparam));
	g_graphSettings.middley = g_strFunc(g_graphSettings.middlex);
	RedrawRequest();
	FillCoordTextBoxes();
}
void Ev_MouseMove(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	int x, y;
	x = LOWORD(lparam);
	y = HIWORD(lparam);
	if (x < (int)g_graphSettings.width)
	{
		if (wparam & MK_LBUTTON)
		{
			g_graphSettings.middlex += (g_graphSettings.mx - x) / g_graphSettings.Zoom();
			g_graphSettings.middley -= (g_graphSettings.my - y) / g_graphSettings.Zoom();
			g_graphSettings.mx = x;
			g_graphSettings.my = y;
			RedrawRequest();
		}
		else
		{
			g_graphSettings.mx = x;
			g_graphSettings.my = y;
			if (wparam & MK_MBUTTON)
			{
				int dx = x - (int)(g_graphSettings.width / 2.0);
				int dy = y - (int)(g_graphSettings.height / 2.0);
				double dist = sqrt(dx*dx + dy * dy);
				g_graphSettings.widthInXAxisUnit = g_graphSettings.prevwixau* dist / g_graphSettings.distanceFromMiddle;
				RedrawRequest();
			}
			else
			{
				FillCoordTextBoxes();
			}
		}
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_COMMAND:
		Ev_Command(hwnd, wparam);
		return 0;
	case WM_CREATE:
		Ev_Create(hwnd);
		return 0;
	case WM_PAINT:
		Ev_Paint(hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_MOUSEWHEEL:
		Ev_MouseWheel(hwnd, wparam);
		return 0;
	case WM_MBUTTONDOWN:
		Ev_MMouseButton(lparam);
		break;
	case WM_RBUTTONDOWN:
		Ev_RMouseButton(hwnd, lparam);
		break;
	case WM_MOUSEMOVE:
		Ev_MouseMove(hwnd, wparam, lparam);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR szCMLine, int iCmdShow)
{
	const WCHAR appName[] = L"GraphApp";
	WNDCLASS wc;
	MSG msg;
	HBRUSH brush = CreateSolidBrush(0x0f0f0f);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = brush;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = appName;

	RegisterClass(&wc);
	g_mainWindow = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, appName, L"Grapher", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, GetModuleHandle(NULL), NULL);

	g_functionInput = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
		10, 130, 240, 24, g_mainWindow, (HMENU)ID_FUNCTION_INPUT, GetModuleHandle(NULL), NULL);
	SetWindowText(g_functionInput, g_functionStr.c_str());	//posts text change event which draws the function

	g_resetButton = CreateWindowEx(WS_EX_CLIENTEDGE, L"button", L"Reset", WS_VISIBLE | WS_CHILD | WS_BORDER,
		10, 160, 240, 24, g_mainWindow, (HMENU)ID_RESET_BUTTON, GetModuleHandle(NULL), NULL);

	g_xPosText = CreateWindowEx(WS_EX_CLIENTEDGE, L"static", L"0", WS_VISIBLE | WS_CHILD,
		10, 40, 240, 22, g_mainWindow, NULL, GetModuleHandle(NULL), NULL);

	g_yPosText = CreateWindowEx(WS_EX_CLIENTEDGE, L"static", L"0", WS_VISIBLE | WS_CHILD,
		10, 64, 240, 22, g_mainWindow, NULL, GetModuleHandle(NULL), NULL);

	g_fxPosText = CreateWindowEx(WS_EX_CLIENTEDGE, L"static", L"0", WS_VISIBLE | WS_CHILD,
		10, 88, 240, 22, g_mainWindow, NULL, GetModuleHandle(NULL), NULL);

	ShowWindow(g_mainWindow, SW_SHOW);
	UpdateWindow(g_mainWindow);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	DeleteObject(brush);

	return msg.wParam;
}