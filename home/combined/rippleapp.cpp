#include "rippleapp.h"

#define ID_TIMER1 1

namespace rpl
{
	void RippleApp::CreateBuffers()
	{
		for (int b = 0; b < 2; b++)
		{
			m_buffers[b] = new COLORREF*[m_height];
			*m_buffers[b] = new COLORREF[m_height*m_width];
			memset(*m_buffers[b], 0, sizeof(COLORREF)*m_height*m_width);
			for (LONG i = 1; i < m_height; i++)
				m_buffers[b][i] = &(m_buffers[b][i - 1][m_width]);
		}
	}
	void RippleApp::DeleteBuffers()
	{
		for (int b = 0; b < 2; b++)
		{
			if (m_buffers[b])
			{
				delete[] * m_buffers[b];
				delete[] m_buffers[b];
				m_buffers[b] = nullptr;
			}
		}
	}
	void RippleApp::StartRipple(int x, int y)
	{
		RECT rect;
		GetClientRect(m_hwnd, &rect);
		m_buffers[m_currentBuffer][y*m_height / rect.bottom][x*m_width / rect.right] = 0xffffffff;
	}
	void RippleApp::DrawFrame()
	{
		int prevBuffer = m_currentBuffer;
		m_currentBuffer = m_currentBuffer ? 0 : 1;
		int val;
		for (int x = 1; x < m_width - 1; x++)
		{
			for (int y = 1; y < m_height - 1; y++)
			{
				val = 0;
				val += m_buffers[prevBuffer][y][x - 1] & 0xff;
				val += m_buffers[prevBuffer][y][x + 1] & 0xff;
				val += m_buffers[prevBuffer][y + 1][x] & 0xff;
				val += m_buffers[prevBuffer][y - 1][x] & 0xff;
				val /= 2;
				val -= m_buffers[m_currentBuffer][y][x] & 0xff;
				val = (int)((double)val * m_damping);
				if (val < 0)
					m_buffers[m_currentBuffer][y][x] = RGB(0x0, 0x0, 0x0);
				else if (val > 0xff)
					m_buffers[m_currentBuffer][y][x] = RGB(0xff, 0xff, 0xff);
				else
					m_buffers[m_currentBuffer][y][x] = RGB(val, val, val);
			}
		}
	}
	void RippleApp::Paint()
	{
		PAINTSTRUCT ps;
		RECT rect;
		HDC hdc = BeginPaint(m_hwnd, &ps);
		GetClientRect(m_hwnd, &rect);
		HBITMAP hbmp = CreateBitmap(m_width, m_height, 1, 32, *m_buffers[m_currentBuffer]);
		HDC hmem = CreateCompatibleDC(hdc);
		SelectObject(hmem, hbmp);
		StretchBlt(hdc, 0, 0, rect.right, rect.bottom, hmem, 0, 0, m_width, m_height, SRCCOPY);
		DeleteObject(hmem);
		EndPaint(m_hwnd, &ps);
	}
	RippleApp::RippleApp(HWND hwnd) :AppBase(hwnd)
	{
		m_damping = 0.95;
		m_width = 640;
		m_height = 480;
		m_currentBuffer = 0;
		CreateBuffers();

		SetWindowSizeTitle(m_width, m_height, L"Ripple");
		InvalidateRect(hwnd, NULL, FALSE);
		SetTimer(hwnd, ID_TIMER1, 50, NULL);
	}
	RippleApp::~RippleApp()
	{
		KillTimer(m_hwnd, ID_TIMER1);
		DeleteBuffers();
	}
	void RippleApp::MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case WM_TIMER:
			DrawFrame();
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case WM_PAINT:
			Paint();
			break;
		case WM_MOUSEMOVE:
			if (wparam & MK_LBUTTON)
				StartRipple(LOWORD(lparam), HIWORD(lparam));
			break;
		case WM_LBUTTONDOWN:
			StartRipple(LOWORD(lparam), HIWORD(lparam));
			break;
		}
	}
	LPCWSTR RippleApp::HelpDialogText()
	{
		return L"\
Water ripple\n\
Press (and drag) left button to ripple water\n";
	}
}