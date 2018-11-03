#pragma once

#include "appbase.h"
#include <array>

namespace gof
{
	class GameOfLifeApp :public AppBase
	{
		std::array<std::array<bool, 48>, 64> m_grid[2];
		bool m_running;
		HBRUSH m_deadColor;
		HBRUSH m_aliveColor;
		int m_currentGrid;
		double m_rate;

	private:
		inline size_t W() { return m_grid[0].size(); }
		inline size_t H() { return m_grid[0][0].size(); }
		void Init();
		void Update();
		void Paint();
		void SetCell(bool alive, int mx, int my);

	public:
		GameOfLifeApp(HWND hwnd);
		virtual ~GameOfLifeApp();
		virtual void MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;
		virtual LPCWSTR HelpDialogText() override;
	};
}