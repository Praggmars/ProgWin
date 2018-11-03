#pragma once

#include "appbase.h"

namespace sdk
{
	class Sudoku
	{
		int m_numbers[9][9];
		bool m_isClue[9][9];

	public:
		Sudoku();
		Sudoku(const Sudoku& other);

		void Load(int sudoku[9][9]);
		void Load(int sudoku[81]);

		inline void setNumberClue(int x, int y, int n) { m_numbers[x][y] = n; m_isClue[x][y] = (bool)n; }
		inline void setNumber(int x, int y, int n) { m_numbers[x][y] = n; m_isClue[x][y] = false; }
		inline int getNumber(int x, int y) { return m_numbers[x][y]; }
		inline bool isClue(int x, int y) { return m_isClue[x][y]; }
		void Clear();
		Sudoku& operator=(const Sudoku& other);
	};

	class SudokuApp :public AppBase
	{
		Sudoku m_sudoku;
		HPEN m_whitePenThin;
		HPEN m_whitePenThick;

		POINT m_selectedCell;
		bool m_creating;

		HWND m_solveButton;
		HWND m_restartButton;
		HWND m_clearButton;
		HWND m_createButton;

	private:
		void DrawGrid(HDC hdc, int size);
		void DrawSudoku(HDC hdc, int size);
		void Paint();
		void ResizeEvent();
		void CommandEvent(WPARAM cmd);
		void ClickEvent(int x, int y);
		void CharEvent(WPARAM keyCode);
		void KeyDownEvent(WPARAM keyCode);

	public:
		SudokuApp(HWND hwnd);
		virtual ~SudokuApp();
		virtual void MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;
		virtual LPCWSTR HelpDialogText() override;
	};
}