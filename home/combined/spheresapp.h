#pragma once

#include "appbase.h"
#include <array>
#include <vector>

namespace sph
{

	class SpheresApp :public AppBase
	{
		struct Point
		{
			double x;
			double y;
		};

		double m_time;
		std::array<std::array<std::vector<Point>, 9>, 18> m_spheres;

	private:
		void DrawNextLines();
		void Paint();

	public:
		SpheresApp(HWND hwnd);
		virtual ~SpheresApp();
		virtual void MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;
		virtual LPCWSTR HelpDialogText() override;
	};
}