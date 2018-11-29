#include "sudokuapp.h"
#include <string>

#define ID_SOLVE_BUTTON 1
#define ID_RESTART_BUTTON 2
#define ID_CLEAR_BUTTON 3
#define ID_CREATE_BUTTON 4

#define MAX_SOLUTION_SEARCH 0

namespace sdk
{
	class SudokuSolver
	{
		class SudokuCell;
		struct SudokuPart
		{
			SudokuCell *cells[9];
			bool included[9];

			SudokuPart()
				:cells(), included() {}
			void CopyDataFrom(const SudokuPart& other)
			{
				memcpy(included, other.included, sizeof(SudokuPart::included));
			}
		};
		class SudokuCell
		{
			int m_value;
			int m_excludedCount;
			bool m_excluded[9];
			SudokuPart *m_row;
			SudokuPart *m_col;
			SudokuPart *m_squ;
			SudokuSolver *m_sudoku;

		public:
			void Init(int index, SudokuSolver *parent)
			{
				m_value = 0;
				m_excludedCount = 0;
				memset(m_excluded, false, sizeof(m_excluded));

				m_sudoku = parent;
				m_row = &m_sudoku->m_rows[index / 9];
				m_col = &m_sudoku->m_cols[index % 9];
				m_squ = &m_sudoku->m_squs[(index / 27) * 3 + (index % 9) / 3];
				m_row->cells[index % 9] = this;
				m_col->cells[index / 9] = this;
				m_squ->cells[(index % 3) + (index / 9) % 3 * 3] = this;
			}
			void CopyDataFrom(const SudokuCell& other)
			{
				m_value = other.m_value;
				m_excludedCount = other.m_excludedCount;
				memcpy(m_excluded, other.m_excluded, sizeof(SudokuCell::m_excluded));
			}
			bool GiveValue(int value) {
				if (value > 0 && value < 10)
				{
					if (m_row->included[value - 1] || m_col->included[value - 1] || m_squ->included[value - 1])
						return false;
					m_value = value;
					for (int i = 0; i < 9; i++)
						m_excluded[i] = (i != value - 1);
					m_excludedCount = 8;
					m_row->included[value - 1] = true;
					m_col->included[value - 1] = true;
					m_squ->included[value - 1] = true;
					m_sudoku->m_emptyCellCount--;
				}
				return true;
			}
			bool ExcludeValue(int value)
			{
				if (value == 0)
					return true;
				if (m_value == value)
					return false;
				if (!m_excluded[value - 1])
				{
					m_excluded[value - 1] = true;
					m_excludedCount++;
					if (m_excludedCount == 8)
					{
						for (int i = 0; i < 9; i++)
						{
							if (!m_excluded[i]) {
								if (m_row->included[i] || m_col->included[i] || m_squ->included[i])
									return false;
								m_value = i + 1;
								m_row->included[i] = true;
								m_col->included[i] = true;
								m_squ->included[i] = true;
								m_sudoku->m_changed = true;
								m_sudoku->m_emptyCellCount--;
							}
						}
					}
				}
				return true;
			}
			bool Check()
			{
				for (int i = 0; i < 9; i++)
				{
					if (!ExcludeValue(m_row->cells[i]->m_value))
						return false;
					if (Filled())
						return true;
					if (!ExcludeValue(m_col->cells[i]->m_value))
						return false;
					if (Filled())
						return true;
					if (!ExcludeValue(m_squ->cells[i]->m_value))
						return false;
					if (Filled())
						return true;
				}
				return true;
			}
			bool Filled()
			{
				return m_value != 0;
			}
			bool isExcluded(int n)
			{
				return m_excluded[n];
			}
			int getExcludedCount()
			{
				return m_excludedCount;
			}
			int getValue()
			{
				return m_value;
			}
		};

		bool m_solvable;
		int m_emptyCellCount;
		bool m_changed;
		SudokuCell m_grid[81];
		SudokuPart m_rows[9];
		SudokuPart m_cols[9];
		SudokuPart m_squs[9];

	private:
		SudokuSolver(Sudoku& sudoku)
		{
			InitParts();
			for (int y = 0; y < 9; y++)
				for (int x = 0; x < 9; x++)
					m_solvable &= m_grid[x + 9 * y].GiveValue(sudoku.getNumber(x, y));
		}
		SudokuSolver(const SudokuSolver& other)
		{
			InitParts();
			for (int i = 0; i < 81; i++)
				m_grid[i].CopyDataFrom(other.m_grid[i]);
			for (int i = 0; i < 9; i++)
			{
				m_rows[i].CopyDataFrom(other.m_rows[i]);
				m_cols[i].CopyDataFrom(other.m_cols[i]);
				m_squs[i].CopyDataFrom(other.m_squs[i]);
			}
			m_emptyCellCount = other.m_emptyCellCount;
			m_solvable = other.m_solvable;
			m_changed = other.m_changed;
		}
		void InitParts()
		{
			for (int i = 0; i < 81; i++)
				m_grid[i].Init(i, this);
			m_emptyCellCount = 81;
			m_solvable = true;
		}
		void CopyDataFrom(const SudokuSolver& other)
		{
			for (int i = 0; i < 81; i++)
				m_grid[i].CopyDataFrom(other.m_grid[i]);
			for (int i = 0; i < 9; i++)
			{
				m_rows[i].CopyDataFrom(other.m_rows[i]);
				m_cols[i].CopyDataFrom(other.m_cols[i]);
				m_squs[i].CopyDataFrom(other.m_squs[i]);
			}
			m_emptyCellCount = other.m_emptyCellCount;
			m_solvable = other.m_solvable;
			m_changed = other.m_changed;
		}
		int SolveSudoku()
		{
			if (!m_solvable)
				return 0;
			while (m_emptyCellCount != 0)
			{
				m_changed = false;
				for (SudokuCell& cell : m_grid)
					if (!cell.Filled())
						if (!cell.Check())
							return 0;
				if (!m_changed)
					return TryAgain();
			}
			return 1;
		}
		int TryAgain()
		{
			int solutions = 0;
			int index = SearchBestChance();
			if (index < 0)
				return 0;
			SudokuSolver save(*this);
			for (int value = 1; value < 10; value++)
			{
				if (!save.m_grid[index].isExcluded(value - 1))
				{
					m_grid[index].GiveValue(value);
					int newSolutions = SolveSudoku();
					if (newSolutions)
					{
						solutions += newSolutions;
						if (solutions > MAX_SOLUTION_SEARCH)
							return solutions;
					}
					if (!save.m_grid[index].ExcludeValue(value))
						return solutions;
					CopyDataFrom(save);
				}
			}
			return solutions;
		}
		int SearchBestChance() {
			int best = -1;
			int index = -1;
			for (int i = 0; i < 81; i++)
			{
				if (!m_grid[i].Filled() && m_grid[i].getExcludedCount() > best)
				{
					best = m_grid[i].getExcludedCount();
					index = i;
				}
			}
			return index;
		}

	public:
		static int Solve(Sudoku& sudoku)
		{
			SudokuSolver solver(sudoku);
			int solutions = solver.SolveSudoku();
			if (solutions)
			{
				for (int y = 0; y < 9; y++)
					for (int x = 0; x < 9; x++)
						if (!sudoku.getNumber(x, y))
							sudoku.setNumber(x, y, solver.m_grid[x + 9 * y].getValue());
			}
			return solutions;
		}
	};

	void SudokuApp::DrawGrid(HDC hdc, int size)
	{
		for (int i = 0; i < 10; i++)
		{
			SelectObject(hdc, (i % 3) ? m_whitePenThin : m_whitePenThick);
			MoveToEx(hdc, 0, i * size / 9, NULL);
			LineTo(hdc, size, i * size / 9);
			MoveToEx(hdc, i * size / 9, 0, NULL);
			LineTo(hdc, i * size / 9, size);
		}
	}
	void SudokuApp::DrawSudoku(HDC hdc, int size)
	{
		LOGFONT logfont{};
		HFONT hfont;
		logfont.lfHeight = size / 9;
		logfont.lfWidth = size / 18;
		lstrcpyW(logfont.lfFaceName, L"consolas");
		hfont = CreateFontIndirect(&logfont);
		SetBkMode(hdc, TRANSPARENT);
		SelectObject(hdc, hfont);
		for (int y = 0; y < 9; y++)
		{
			for (int x = 0; x < 9; x++)
			{
				if (m_sudoku.getNumber(x, y))
				{
					SetTextColor(hdc, m_sudoku.isClue(x, y) ? RGB(0xff, 0xff, 0xff) : RGB(0x3f, 0x5f, 0xff));
					WCHAR ch = (WCHAR)m_sudoku.getNumber(x, y) + '0';
					TextOut(hdc, x * size / 9 + size / 36, y * size / 9, &ch, 1);
				}
			}
		}
		if (m_selectedCell.x >= 0 && m_selectedCell.y >= 0)
		{
			SetTextColor(hdc, (m_creating || m_sudoku.isClue(m_selectedCell.x, m_selectedCell.y)) ? RGB(0xff, 0xff, 0xff) : RGB(0x3f, 0x3f, 0xff));
			WCHAR ch = '_';
			TextOut(hdc, m_selectedCell.x * size / 9 + size / 36, m_selectedCell.y * size / 9, &ch, 1);
		}
		DeleteObject(hfont);
	}
	void SudokuApp::Paint()
	{
		PAINTSTRUCT ps;
		RECT rect;
		GetClientRect(m_hwnd, &rect);
		int size = min(rect.bottom, rect.right - 100);
		HDC hdc = BeginPaint(m_hwnd, &ps);

		DrawGrid(hdc, size);
		DrawSudoku(hdc, size);

		EndPaint(m_hwnd, &ps);
	}
	void SudokuApp::ResizeEvent()
	{
		RECT rect;
		GetClientRect(m_hwnd, &rect);
		int size = min(rect.bottom, rect.right - 100);
		MoveWindow(m_solveButton, size + 10, 10, 80, 24, FALSE);
		MoveWindow(m_restartButton, size + 10, 40, 80, 24, FALSE);
		MoveWindow(m_clearButton, size + 10, 70, 80, 24, FALSE);
		MoveWindow(m_createButton, size + 10, 100, 80, 24, FALSE);
		MoveWindow(m_solutionLabel, size + 10, 130, 80, 36, FALSE);
	}
	void SudokuApp::CommandEvent(WPARAM cmd)
	{
		ShowWindow(m_solutionLabel, FALSE);
		switch (cmd)
		{
		case ID_SOLVE_BUTTON:
		{
			int solutions = SudokuSolver::Solve(m_sudoku);
			if (solutions > MAX_SOLUTION_SEARCH)
				SetWindowText(m_solutionLabel, (L"Solutions\nover " + std::to_wstring(MAX_SOLUTION_SEARCH)).c_str());
			else
				SetWindowText(m_solutionLabel, (L"Solutions\n" + std::to_wstring(solutions)).c_str());
			ShowWindow(m_solutionLabel, TRUE);
		}
		break;
		case ID_RESTART_BUTTON:
			for (int y = 0; y < 9; y++)
				for (int x = 0; x < 9; x++)
					if (!m_sudoku.isClue(x, y))
						m_sudoku.setNumber(x, y, 0);
			break;
		case ID_CLEAR_BUTTON:
			m_sudoku.Clear();
			break;
		case ID_CREATE_BUTTON:
			if (m_creating)
				SetWindowText(m_createButton, L"Create");
			else
				SetWindowText(m_createButton, L"Play");
			m_creating = !m_creating;
			break;
		}
		InvalidateRect(m_hwnd, NULL, TRUE);
		SetFocus(m_hwnd);
	}
	void SudokuApp::LButtonDownEvent(int x, int y)
	{
		RECT rect;
		GetClientRect(m_hwnd, &rect);
		int size = min(rect.bottom, rect.right - 100);
		x /= size / 9;
		y /= size / 9;
		if (x >= 0 && x < 9 && y >= 0 && y < 9 &&
			!(m_selectedCell.x == x && m_selectedCell.y == y))
		{
			m_selectedCell.x = x;
			m_selectedCell.y = y;
		}
		else
		{
			m_selectedCell.x = -1;
			m_selectedCell.y = -1;
		}
		InvalidateRect(m_hwnd, NULL, TRUE);
	}
	void SudokuApp::CharEvent(WPARAM keyCode)
	{
		if (m_selectedCell.x >= 0 && m_selectedCell.y >= 0)
		{
			if (keyCode >= '0' && keyCode <= '9')
			{
				if (m_creating)
					m_sudoku.setNumberClue(m_selectedCell.x, m_selectedCell.y, keyCode - '0');
				else if (!m_sudoku.isClue(m_selectedCell.x, m_selectedCell.y))
					m_sudoku.setNumber(m_selectedCell.x, m_selectedCell.y, keyCode - '0');
				InvalidateRect(m_hwnd, NULL, TRUE);
			}
		}
	}
	void SudokuApp::KeyDownEvent(WPARAM keyCode)
	{
		if (m_selectedCell.x < 0 && m_selectedCell.y < 0)
			return;
		switch (keyCode)
		{
		case VK_UP:
			if (m_selectedCell.y > 0)
				m_selectedCell.y--;
			else
			{
				m_selectedCell.y = 8;
				if (m_selectedCell.x > 0)
					m_selectedCell.x--;
				else
					m_selectedCell.x = 8;
			}
			InvalidateRect(m_hwnd, NULL, TRUE);
			break;
		case VK_DOWN:
			if (m_selectedCell.y < 8)
				m_selectedCell.y++;
			else
			{
				m_selectedCell.y = 0;
				if (m_selectedCell.x < 8)
					m_selectedCell.x++;
				else
					m_selectedCell.x = 0;
			}
			InvalidateRect(m_hwnd, NULL, TRUE);
			break;
		case VK_LEFT:
			if (m_selectedCell.x > 0)
				m_selectedCell.x--;
			else
			{
				m_selectedCell.x = 8;
				if (m_selectedCell.y > 0)
					m_selectedCell.y--;
				else
					m_selectedCell.y = 8;
			}
			InvalidateRect(m_hwnd, NULL, TRUE);
			break;
		case VK_RIGHT:
			if (m_selectedCell.x < 8)
				m_selectedCell.x++;
			else
			{
				m_selectedCell.x = 0;
				if (m_selectedCell.y < 8)
					m_selectedCell.y++;
				else
					m_selectedCell.y = 0;
			}
			InvalidateRect(m_hwnd, NULL, TRUE);
			break;
		}
	}
	SudokuApp::SudokuApp(HWND hwnd)
		:AppBase(hwnd)
	{
		m_whitePenThin = CreatePen(PS_SOLID, 1, RGB(0xff, 0xff, 0xff));
		m_whitePenThick = CreatePen(PS_SOLID, 3, RGB(0xff, 0xff, 0xff));

		int numbers[] = {
			5,3,0,0,7,0,0,0,0,
			6,0,0,1,9,5,0,0,0,
			0,9,8,0,0,0,0,6,0,
			8,0,0,0,6,0,0,0,3,
			4,0,0,8,0,3,0,0,1,
			7,0,0,0,2,0,0,0,6,
			0,6,0,0,0,0,2,8,0,
			0,0,0,4,1,9,0,0,5,
			0,0,0,0,8,0,0,7,9
		};
		m_sudoku.Load(numbers);

		RECT rect;
		GetClientRect(m_hwnd, &rect);
		int size = min(rect.bottom, rect.right - 100);
		m_solveButton = CreateWindowEx(WS_EX_CLIENTEDGE, L"button", L"Solve", WS_VISIBLE | WS_CHILD | WS_BORDER,
			size + 10, 10, 80, 24, hwnd, (HMENU)ID_SOLVE_BUTTON, GetModuleHandle(NULL), NULL);
		m_restartButton = CreateWindowEx(WS_EX_CLIENTEDGE, L"button", L"Restart", WS_VISIBLE | WS_CHILD | WS_BORDER,
			size + 10, 40, 80, 24, hwnd, (HMENU)ID_RESTART_BUTTON, GetModuleHandle(NULL), NULL);
		m_clearButton = CreateWindowEx(WS_EX_CLIENTEDGE, L"button", L"Clear", WS_VISIBLE | WS_CHILD | WS_BORDER,
			size + 10, 70, 80, 24, hwnd, (HMENU)ID_CLEAR_BUTTON, GetModuleHandle(NULL), NULL);
		m_createButton = CreateWindowEx(WS_EX_CLIENTEDGE, L"button", L"Create", WS_VISIBLE | WS_CHILD | WS_BORDER,
			size + 10, 100, 80, 24, hwnd, (HMENU)ID_CREATE_BUTTON, GetModuleHandle(NULL), NULL);
		m_solutionLabel = CreateWindowEx(WS_EX_CLIENTEDGE, L"static", L"Solutions\n", WS_CHILD | SS_CENTER,
			size + 10, 130, 80, 36, hwnd, NULL, GetModuleHandle(NULL), NULL);

		m_selectedCell.x = -1;
		m_selectedCell.y = -1;
		m_creating = false;

		SetWindowTitle(L"Sudoku");
		InvalidateRect(hwnd, NULL, TRUE);
	}
	SudokuApp::~SudokuApp()
	{
		DeleteObject(m_whitePenThin);
		DeleteObject(m_whitePenThick);
		DestroyWindow(m_solveButton);
		DestroyWindow(m_restartButton);
		DestroyWindow(m_clearButton);
		DestroyWindow(m_createButton);
		DestroyWindow(m_solutionLabel);
	}
	void SudokuApp::MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case WM_COMMAND:
			CommandEvent(wparam);
			break;
		case WM_PAINT:
			Paint();
			break;
		case WM_SIZE:
			ResizeEvent();
			break;
		case WM_LBUTTONDOWN:
			LButtonDownEvent(LOWORD(lparam), HIWORD(lparam));
			break;
		case WM_CHAR:
			CharEvent(wparam);
			break;
		case WM_KEYDOWN:
			KeyDownEvent(wparam);
			break;
		}
	}
	LPCWSTR SudokuApp::HelpDialogText()
	{
		return L"Sudoku\n\
Select a square and type in a number\n\
Use arroy keys to navigate\n\
Type 0 to clear the square";
	}

#pragma region Sudoku

	Sudoku::Sudoku()
	{
		Clear();
	}
	Sudoku::Sudoku(const Sudoku& other)
	{
		memcpy(this, &other, sizeof(Sudoku));
	}
	void Sudoku::Load(int sudoku[9][9])
	{
		for (int y = 0; y < 9; y++)
		{
			for (int x = 0; x < 9; x++)
			{
				if (sudoku[x][y] > 0 && sudoku[x][y] < 10)
				{
					m_numbers[x][y] = sudoku[x][y];
					m_isClue[x][y] = true;
				}
			}
		}
	}
	void Sudoku::Load(int sudoku[81])
	{
		for (int y = 0; y < 9; y++)
		{
			for (int x = 0; x < 9; x++)
			{
				if (sudoku[x + 9 * y] > 0 && sudoku[x + 9 * y] < 10)
				{
					m_numbers[x][y] = sudoku[x + 9 * y];
					m_isClue[x][y] = true;
				}
			}
		}
	}
	void Sudoku::Clear()
	{
		memset(this, 0, sizeof(*this));
	}
	Sudoku& Sudoku::operator=(const Sudoku& other)
	{
		memcpy(this, &other, sizeof(Sudoku));
		return *this;
	}

#pragma endregion

}
