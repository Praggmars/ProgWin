#include "functionapp.h"

#define ID_FUNCTION_INPUT 1
#define ID_RESET_BUTTON 2

namespace fnc
{
	void FunctionApp::FillCoordTextBoxes()
	{
		SetWindowText(m_xPosText, (L"x = " + std::to_wstring(m_graphSettings.ToGraphXCoord(m_graphSettings.mx))).c_str());
		SetWindowText(m_yPosText, (L"y = " + std::to_wstring(m_graphSettings.ToGraphYCoord(m_graphSettings.my))).c_str());
		SetWindowText(m_fxPosText, (L"f(x) = " + std::to_wstring(m_strFunc(m_graphSettings.ToGraphXCoord(m_graphSettings.mx)))).c_str());
	}
	void FunctionApp::DrawFunction(HDC hdc, std::function<double(double)> f)
	{
		bool valid = false;
		double x, y;
		int draw_x, draw_y;
		HPEN hpen = CreatePen(PS_SOLID, 5, 0x1f7fff);
		SelectObject(hdc, hpen);

		for (int i = 2; i < m_graphSettings.width - 2; i++)
		{
			x = m_graphSettings.ToGraphXCoord(i);
			y = f(x);
			if (isnan(y) || isinf(y))
			{
				valid = false;
			}
			else
			{
				draw_x = m_graphSettings.ToScreenXCoordClamp(x);
				draw_y = m_graphSettings.ToScreenYCoordClamp(y);
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
	void FunctionApp::DrawAxises(HDC hdc)
	{
		HPEN thin = CreatePen(PS_SOLID, 1, 0xffffff);
		HPEN thick = CreatePen(PS_SOLID, 3, 0xffffff);
		SelectObject(hdc, thin);

		int draw_coord;
		int guard;
		int maxLines = 1000;
		double linedist = pow(2.0, floor(log2(m_graphSettings.widthInXAxisUnit))) / 8.0;

		double start = ceil(-m_graphSettings.ToScreenXCoord(0.0) / m_graphSettings.Zoom() / linedist)*linedist;
		double end = ceil((m_graphSettings.width - m_graphSettings.ToScreenXCoord(0.0)) / m_graphSettings.Zoom() / linedist)*linedist;
		guard = 0;
		for (double x = start; x < end; x += linedist)
		{
			draw_coord = (int)(x * m_graphSettings.Zoom() + m_graphSettings.ToScreenXCoord(0.0));
			MoveToEx(hdc, draw_coord, 0, NULL);
			LineTo(hdc, draw_coord, (int)m_graphSettings.height);
			if (++guard > maxLines)
				break;
		}
		start = ceil(-m_graphSettings.ToScreenYCoord(0.0) / m_graphSettings.Zoom() / linedist)*linedist;
		end = ceil((m_graphSettings.height - m_graphSettings.ToScreenYCoord(0.0)) / m_graphSettings.Zoom() / linedist)*linedist;
		guard = 0;
		for (double y = start; y < end; y += linedist)
		{
			draw_coord = (int)(y * m_graphSettings.Zoom() + m_graphSettings.ToScreenYCoord(0.0));
			MoveToEx(hdc, 0, draw_coord, NULL);
			LineTo(hdc, (int)m_graphSettings.width, draw_coord);
			if (++guard > maxLines)
				break;
		}

		SelectObject(hdc, thick);
		if (m_graphSettings.ToScreenYCoord(0.0) < m_graphSettings.height && m_graphSettings.ToScreenYCoord(0.0) >= 0)
		{
			MoveToEx(hdc, 0, m_graphSettings.ToScreenYCoord(0.0), NULL);
			LineTo(hdc, (int)m_graphSettings.width - 2, m_graphSettings.ToScreenYCoord(0.0));
		}
		if (m_graphSettings.ToScreenXCoord(0.0) < m_graphSettings.width && m_graphSettings.ToScreenXCoord(0.0) >= 0)
		{
			MoveToEx(hdc, m_graphSettings.ToScreenXCoord(0.0), 0, NULL);
			LineTo(hdc, m_graphSettings.ToScreenXCoord(0.0), (int)m_graphSettings.height);
		}

		DeleteObject(thin);
		DeleteObject(thick);
	}
	void FunctionApp::LoadFunction()
	{
		WCHAR str[512];
		GetWindowText(m_functionInput, str, 511);
		m_strFunc.LoadFunction(str);
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
	void FunctionApp::Paint()
	{
		PAINTSTRUCT ps;
		RECT rect;
		GetClientRect(m_hwnd, &rect);
		rect.right = max(rect.right - 240, 1);
		m_graphSettings.width = max(rect.right, 1);
		m_graphSettings.height = max(rect.bottom, 1);

		HDC hdc = BeginPaint(m_hwnd, &ps);
		HBRUSH brush = CreateSolidBrush(0x1f1f1f);
		FillRect(hdc, &rect, brush);
		DrawAxises(hdc);
		DrawFunction(hdc, [this](double x)->double {return m_strFunc(x); });
		DeleteObject(brush);
		EndPaint(m_hwnd, &ps);
	}
	void FunctionApp::Ev_Command(HWND hwnd, WPARAM wparam)
	{
		if (LOWORD(wparam) == ID_FUNCTION_INPUT && HIWORD(wparam) == EN_CHANGE)
			LoadFunction();
		if (LOWORD(wparam) == ID_RESET_BUTTON)
		{
			m_graphSettings.DefaultSettings();
			InvalidateRect(hwnd, NULL, FALSE);
		}
	}
	void FunctionApp::Ev_MouseWheel(HWND hwnd, WPARAM wparam)
	{
		if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
			m_graphSettings.widthInXAxisUnit /= 1.1;
		else
			m_graphSettings.widthInXAxisUnit *= 1.1;
		InvalidateRect(hwnd, NULL, FALSE);
		FillCoordTextBoxes();
	}
	void FunctionApp::Ev_MMouseButton(LPARAM lparam)
	{
		int dx = LOWORD(lparam) - (int)(m_graphSettings.width / 2.0);
		int dy = HIWORD(lparam) - (int)(m_graphSettings.height / 2.0);
		m_graphSettings.distanceFromMiddle = sqrt(dx*dx + dy * dy);
		m_graphSettings.prevwixau = m_graphSettings.widthInXAxisUnit;
	}
	void FunctionApp::Ev_RMouseButton(HWND hwnd, LPARAM lparam)
	{
		m_graphSettings.middlex = m_graphSettings.ToGraphXCoord(LOWORD(lparam));
		m_graphSettings.middley = m_strFunc(m_graphSettings.middlex);
		InvalidateRect(hwnd, NULL, FALSE);
		FillCoordTextBoxes();
	}
	void FunctionApp::Ev_MouseMove(HWND hwnd, WPARAM wparam, LPARAM lparam)
	{
		int x, y;
		x = LOWORD(lparam);
		y = HIWORD(lparam);
		if (x < (int)m_graphSettings.width)
		{
			if (wparam & MK_LBUTTON)
			{
				m_graphSettings.middlex += (m_graphSettings.mx - x) / m_graphSettings.Zoom();
				m_graphSettings.middley -= (m_graphSettings.my - y) / m_graphSettings.Zoom();
				m_graphSettings.mx = x;
				m_graphSettings.my = y;
				InvalidateRect(hwnd, NULL, FALSE);
			}
			else
			{
				m_graphSettings.mx = x;
				m_graphSettings.my = y;
				if (wparam & MK_MBUTTON)
				{
					int dx = x - (int)(m_graphSettings.width / 2.0);
					int dy = y - (int)(m_graphSettings.height / 2.0);
					double dist = sqrt(dx*dx + dy * dy);
					m_graphSettings.widthInXAxisUnit = m_graphSettings.prevwixau* dist / m_graphSettings.distanceFromMiddle;
					InvalidateRect(hwnd, NULL, FALSE);
				}
				else
				{
					FillCoordTextBoxes();
				}
			}
		}
	}

	void FunctionApp::Ev_Size(HWND hwnd)
	{
		MoveWindow(m_xPosText, (int)m_graphSettings.width + 10, 40, 220, 22, FALSE);
		MoveWindow(m_yPosText, (int)m_graphSettings.width + 10, 64, 220, 22, FALSE);
		MoveWindow(m_fxPosText, (int)m_graphSettings.width + 10, 88, 220, 22, FALSE);
		MoveWindow(m_functionInput, (int)m_graphSettings.width + 10, 130, 220, 24, FALSE);
		MoveWindow(m_resetButton, (int)m_graphSettings.width + 10, 160, 220, 24, FALSE);
	}

	FunctionApp::FunctionApp(HWND hwnd) :AppBase(hwnd)
	{
		m_graphSettings.DefaultSettings();
		RECT rect;
		GetClientRect(m_hwnd, &rect);
		rect.right = max(rect.right - 240, 1);
		m_graphSettings.width = max(rect.right, 1);
		m_graphSettings.height = max(rect.bottom, 1);

		m_functionInput = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
			(int)m_graphSettings.width + 10, 130, 220, 24, hwnd, (HMENU)ID_FUNCTION_INPUT, GetModuleHandle(NULL), NULL);

		m_resetButton = CreateWindowEx(WS_EX_CLIENTEDGE, L"button", L"Reset", WS_VISIBLE | WS_CHILD | WS_BORDER,
			(int)m_graphSettings.width + 10, 160, 220, 24, hwnd, (HMENU)ID_RESET_BUTTON, GetModuleHandle(NULL), NULL);

		m_xPosText = CreateWindowEx(WS_EX_CLIENTEDGE, L"static", L"0", WS_VISIBLE | WS_CHILD,
			(int)m_graphSettings.width + 10, 40, 220, 22, hwnd, NULL, GetModuleHandle(NULL), NULL);

		m_yPosText = CreateWindowEx(WS_EX_CLIENTEDGE, L"static", L"0", WS_VISIBLE | WS_CHILD,
			(int)m_graphSettings.width + 10, 64, 220, 22, hwnd, NULL, GetModuleHandle(NULL), NULL);

		m_fxPosText = CreateWindowEx(WS_EX_CLIENTEDGE, L"static", L"0", WS_VISIBLE | WS_CHILD,
			(int)m_graphSettings.width + 10, 88, 220, 22, hwnd, NULL, GetModuleHandle(NULL), NULL);

		SetWindowTitle(L"Function");
		InvalidateRect(hwnd, NULL, TRUE);
	}
	FunctionApp::~FunctionApp()
	{
		DestroyWindow(m_functionInput);
		DestroyWindow(m_resetButton);
		DestroyWindow(m_xPosText);
		DestroyWindow(m_yPosText);
		DestroyWindow(m_fxPosText);
	}
	void FunctionApp::MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case WM_COMMAND:
			Ev_Command(hwnd, wparam);
			break;
		case WM_PAINT:
			Paint();
			break;
		case WM_MOUSEWHEEL:
			Ev_MouseWheel(hwnd, wparam);
			break;
		case WM_MBUTTONDOWN:
			Ev_MMouseButton(lparam);
			break;
		case WM_RBUTTONDOWN:
			Ev_RMouseButton(hwnd, lparam);
			break;
		case WM_MOUSEMOVE:
			Ev_MouseMove(hwnd, wparam, lparam);
			break;
		case WM_SIZE:
			Ev_Size(hwnd);
			break;
		}
	}
	LPCWSTR FunctionApp::HelpDialogText()
	{
		return L"Function plotter\n\
Type function to the textfield on the right\n\
Grab with left button and move around the function\n\
Scroll mouse wheel to zoom in and out\n\
Right click to jump to [x, f(x)] coordinates";
	}
}