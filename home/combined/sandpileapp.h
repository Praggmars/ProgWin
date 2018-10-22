#pragma once

#include "appbase.h"
#include <array>

namespace spl
{
	constexpr int GRID_SIZE = 31;
	constexpr int STARTING_GRAIN = 1500;

	class SandPileApp :public AppBase
	{
		std::array<std::array<int, GRID_SIZE>, GRID_SIZE> m_grid[2];
		int m_currentGrid;
		std::array<HBRUSH, 5> m_colors;

	private:
		inline size_t W() { return m_grid[0].size(); }
		inline size_t H() { return m_grid[0][0].size(); }
		bool Topple();
		void Paint();

	public:
		SandPileApp(HWND hwnd);
		virtual ~SandPileApp();
		virtual void MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;
		virtual LPCWSTR HelpDialogText() override;
	};
}