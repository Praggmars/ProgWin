#pragma once

#include "appbase.h"
#include <array>

namespace spl
{
	constexpr int GRID_SIZE = 31;
	constexpr int STARTING_GRAIN = 1500;

	class SandPileApp :public AppBase
	{
		std::array<std::array<int, GRID_SIZE>, GRID_SIZE> m_grid;
		std::array<HBRUSH, 5> m_colors;

	private:
		bool Topple();
		void Paint();

	public:
		SandPileApp(HWND hwnd);
		virtual ~SandPileApp();
		virtual void MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;
	};
}