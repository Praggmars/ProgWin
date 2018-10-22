#include "sandpileapp.h"

#define ID_TIMER1 1

namespace spl
{
	bool SandPileApp::Topple()
	{
		bool changed = false;
		int prevGrid = m_currentGrid;
		m_currentGrid = m_currentGrid ? 0 : 1;
		memset(&m_grid[m_currentGrid], 0, sizeof(m_grid[m_currentGrid]));
		for (size_t x = 0; x < W(); x++)
		{
			for (size_t y = 0; y < H(); y++)
			{
				if (m_grid[prevGrid][x][y] > 3)
				{
					m_grid[m_currentGrid][x][y] += m_grid[prevGrid][x][y] - 4;
					m_grid[m_currentGrid][x][y + 1]++;
					m_grid[m_currentGrid][x][y - 1]++;
					m_grid[m_currentGrid][x + 1][y]++;
					m_grid[m_currentGrid][x - 1][y]++;
					changed = true;
				}
				else
				{
					m_grid[m_currentGrid][x][y] += m_grid[prevGrid][x][y];
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

		for (size_t x = 0; x < W(); x++)
		{
			for (size_t y = 0; y < H(); y++)
			{
				if (m_grid[m_currentGrid][x][y] < 4)
					SelectObject(hdc, m_colors[m_grid[m_currentGrid][x][y]]);
				else
					SelectObject(hdc, m_colors[4]);

				Rectangle(hdc, x*size / W() + xoffset, y*size / H() + yoffset,
					(x + 1)*size / W() + xoffset, (y + 1)*size / H() + yoffset);
			}
		}

		EndPaint(m_hwnd, &ps);
	}
	SandPileApp::SandPileApp(HWND hwnd) :AppBase(hwnd)
	{
		m_currentGrid = 0;
		m_grid[m_currentGrid][GRID_SIZE / 2][GRID_SIZE / 2] = STARTING_GRAIN;
		m_colors[0] = CreateSolidBrush(RGB(0x0, 0x0, 0xff));
		m_colors[1] = CreateSolidBrush(RGB(0x0, 0xff, 0xff));
		m_colors[2] = CreateSolidBrush(RGB(0xff, 0xff, 0x0));
		m_colors[3] = CreateSolidBrush(RGB(0x80, 0x0, 0x0));
		m_colors[4] = CreateSolidBrush(RGB(0x0, 0x0, 0x0));
		SetWindowSizeTitle(480, 480, L"Sandpile");
		SetTimer(hwnd, ID_TIMER1, 50, NULL);
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
	LPCWSTR SandPileApp::HelpDialogText()
	{
		return L"Sandpile topple simulation";
	}
}
