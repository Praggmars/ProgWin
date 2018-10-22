#include "gameoflife.h"

#define ID_TIMER1 1

namespace gof
{
	void GameOfLifeApp::Init()
	{
		int rate = (int)(RAND_MAX*m_rate);
		m_running = true;
		m_currentGrid = 0;
		for (size_t x = 0; x < W(); x++)
		{
			for (size_t y = 0; y < H(); y++)
			{
				m_grid[m_currentGrid][x][y] = (rand() < rate);
			}
		}
	}
	void GameOfLifeApp::Update()
	{
		int prevGrid = m_currentGrid;
		m_currentGrid = m_currentGrid ? 0 : 1;
		for (size_t x = 0; x < W(); x++)
		{
			for (size_t y = 0; y < H(); y++)
			{
				int neighbours = 0;
				if (m_grid[prevGrid][(x - 1 + W()) % W()][(y - 1 + H()) % H()]) neighbours++;
				if (m_grid[prevGrid][(x + 0 + W()) % W()][(y - 1 + H()) % H()]) neighbours++;
				if (m_grid[prevGrid][(x + 1 + W()) % W()][(y - 1 + H()) % H()]) neighbours++;
				if (m_grid[prevGrid][(x + 1 + W()) % W()][(y + 0 + H()) % H()]) neighbours++;
				if (m_grid[prevGrid][(x - 1 + W()) % W()][(y + 0 + H()) % H()]) neighbours++;
				if (m_grid[prevGrid][(x - 1 + W()) % W()][(y + 1 + H()) % H()]) neighbours++;
				if (m_grid[prevGrid][(x + 0 + W()) % W()][(y + 1 + H()) % H()]) neighbours++;
				if (m_grid[prevGrid][(x + 1 + W()) % W()][(y + 1 + H()) % H()]) neighbours++;
				if (m_grid[prevGrid][x][y])
					m_grid[m_currentGrid][x][y] = !(neighbours < 2 || neighbours > 3);
				else
					m_grid[m_currentGrid][x][y] = (neighbours == 3);
			}
		}
	}
	void GameOfLifeApp::Paint()
	{
		HDC hdc;
		PAINTSTRUCT ps;
		RECT rect;
		hdc = BeginPaint(m_hwnd, &ps);
		GetClientRect(m_hwnd, &rect);
		double cellSize = min((double)rect.right / (double)W(), (double)rect.bottom / (double)H());
		size_t xoffset = (size_t)(rect.right - cellSize * W()) / 2;
		size_t yoffset = (size_t)(rect.bottom - cellSize * H()) / 2;

		for (size_t x = 0; x < W(); x++)
		{
			for (size_t y = 0; y < H(); y++)
			{
				if (m_grid[m_currentGrid][x][y])
					SelectObject(hdc, m_aliveColor);
				else
					SelectObject(hdc, m_deadColor);
				Rectangle(hdc, (int)(x * cellSize) + xoffset, (int)(y * cellSize) + yoffset,
					(int)((x + 1)*cellSize) + xoffset, (int)((y + 1)*cellSize) + yoffset);
			}
		}
		EndPaint(m_hwnd, &ps);
	}
	void GameOfLifeApp::SetCell(bool alive, int mx, int my)
	{
		RECT rect;
		GetClientRect(m_hwnd, &rect);
		double cellSize = min((double)rect.right / (double)W(), (double)rect.bottom / (double)H());
		size_t xoffset = (size_t)(rect.right - cellSize * W()) / 2;
		size_t yoffset = (size_t)(rect.bottom - cellSize * H()) / 2;
		mx = (mx - xoffset) / (int)cellSize;
		my = (my - yoffset) / (int)cellSize;
		if (mx >= 0 && mx < (int)W() && my >= 0 && my < (int)H())
			m_grid[m_currentGrid][mx][my] = alive;
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
	GameOfLifeApp::GameOfLifeApp(HWND hwnd) :AppBase(hwnd)
	{
		m_rate = 0.375;
		m_aliveColor = CreateSolidBrush(RGB(0x40, 0x40, 0xff));
		m_deadColor = CreateSolidBrush(RGB(0x40, 0x40, 0x40));
		Init();
		int cellSize = (int)min(640.0 / (double)W(), 480.0 / (double)H());
		SetWindowSizeTitle(cellSize * W(), cellSize * H(), L"Game of Life");
		SetTimer(hwnd, ID_TIMER1, 100, NULL);
	}
	GameOfLifeApp::~GameOfLifeApp()
	{
		DeleteObject(m_aliveColor);
		DeleteObject(m_deadColor);
		KillTimer(m_hwnd, ID_TIMER1);
	}
	void GameOfLifeApp::MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case WM_TIMER:
			Update();
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case WM_LBUTTONDOWN:
			SetCell(true, LOWORD(lparam), HIWORD(lparam));
			break;
		case WM_RBUTTONDOWN:
			SetCell(false, LOWORD(lparam), HIWORD(lparam));
			break;
		case WM_MOUSEMOVE:
			if (wparam&MK_RBUTTON)
				SetCell(false, LOWORD(lparam), HIWORD(lparam));
			else if (wparam&MK_LBUTTON)
				SetCell(true, LOWORD(lparam), HIWORD(lparam));
			break;
		case WM_KEYDOWN:
			switch (wparam)
			{
			case VK_SPACE:
				if (m_running)
				{
					KillTimer(hwnd, ID_TIMER1);
					m_running = false;
				}
				else
				{
					SetTimer(hwnd, ID_TIMER1, 100, NULL);
					m_running = true;
				}
				break;
			case 'C':
				memset(&m_grid[m_currentGrid], false, sizeof(m_grid[m_currentGrid]));
				InvalidateRect(m_hwnd, NULL, FALSE);
				break;
			}
			break;
		case WM_PAINT:
			Paint();
			break;
		}
	}
	LPCWSTR GameOfLifeApp::HelpDialogText()
	{
		return L"Conway's Game of Life\n\
Click and drag left button to add active cells\n\
Click and drag right button to add inactive cells\n\
Press spacebar to pause or resume\n\
Press 'C' to clear grid";
	}
}