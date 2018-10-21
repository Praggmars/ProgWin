#include "sandpileapp.h"

#define ID_TIMER1 1

namespace spl
{
	bool SandPileApp::Topple()
	{
		decltype(m_grid) src;
		bool changed = false;
		memcpy(&src, &m_grid, sizeof(m_grid));
		memset(&m_grid, 0, sizeof(m_grid));
		for (size_t x = 0; x < src.size(); x++)
		{
			for (size_t y = 0; y < src[x].size(); y++)
			{
				if (src[x][y] > 3)
				{
					m_grid[x][y] += src[x][y] - 4;
					m_grid[x][y + 1]++;
					m_grid[x][y - 1]++;
					m_grid[x + 1][y]++;
					m_grid[x - 1][y]++;
					changed = true;
				}
				else
				{
					m_grid[x][y] += src[x][y];
				}
			}
		}
		return changed;
	}
	void SandPileApp::Paint()
	{
		HDC hdc;
		PAINTSTRUCT ps;
		RECT rect;
		hdc = BeginPaint(m_hwnd, &ps);
		GetClientRect(m_hwnd, &rect);
		int size = min(rect.right, rect.bottom);
		int xoffset = (rect.right - size) / 2;
		int yoffset = (rect.bottom - size) / 2;

		for (size_t x = 0; x < m_grid.size(); x++)
		{
			for (size_t y = 0; y < m_grid[x].size(); y++)
			{
				if (m_grid[x][y] < 4)
					SelectObject(hdc, m_colors[m_grid[x][y]]);
				else
					SelectObject(hdc, m_colors[4]);

				Rectangle(hdc, x*size / m_grid.size() + xoffset, y*size / m_grid.size() + yoffset,
					(x + 1)*size / m_grid.size() + xoffset, (y + 1)*size / m_grid.size() + yoffset);
			}
		}

		EndPaint(m_hwnd, &ps);
	}
	SandPileApp::SandPileApp(HWND hwnd) :AppBase(hwnd)
	{
		SetTimer(hwnd, ID_TIMER1, 50, NULL);
		m_grid[GRID_SIZE / 2][GRID_SIZE / 2] = STARTING_GRAIN;
		m_colors[0] = CreateSolidBrush(RGB(0x0, 0x0, 0xff));
		m_colors[1] = CreateSolidBrush(RGB(0x0, 0xff, 0xff));
		m_colors[2] = CreateSolidBrush(RGB(0xff, 0xff, 0x0));
		m_colors[3] = CreateSolidBrush(RGB(0x80, 0x0, 0x0));
		m_colors[4] = CreateSolidBrush(RGB(0x0, 0x0, 0x0));
		SetWindowSizeTitle(480, 480, L"Sandpile");
	}
	SandPileApp::~SandPileApp()
	{
		DeleteObject(m_colors[0]);
		DeleteObject(m_colors[1]);
		DeleteObject(m_colors[2]);
		DeleteObject(m_colors[3]);
		DeleteObject(m_colors[4]);
		KillTimer(m_hwnd, ID_TIMER1);
	}
	void SandPileApp::MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case WM_TIMER:
			if (LOWORD(wparam) == ID_TIMER1)
			{
				if (!Topple())
					KillTimer(hwnd, ID_TIMER1);
				InvalidateRect(hwnd, NULL, FALSE);
			}
			break;
		case WM_PAINT:
			Paint();
			break;
		}
	}
}
