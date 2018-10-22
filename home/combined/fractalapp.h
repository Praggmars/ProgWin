#pragma once
#include "appbase.h"
#include "linalg.h"
#include <vector>

namespace frac
{
	class FractalApp :public AppBase
	{
		std::vector<linalg::Vector<double, 2>> m_vertices;
		std::vector<linalg::Vector<double, 2>> m_dots;
		linalg::Vector<double, 2> m_prevDot;
		int m_dotSize;
		int m_vertexSize;

	private:
		inline int X(double x, int w) { return (int)((x + 1) / 2 * w); }
		inline int Y(double y, int h) { return (int)((y - 1) / (-2) * h); }

		void DrawNextDot();
		void Paint();
		void PlaceVertex(int x, int y);
		void RemoveVertex(int x, int y);

	public:
		FractalApp(HWND hwnd);
		virtual ~FractalApp();
		virtual void MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;
		virtual LPCWSTR HelpDialogText() override;
	};
}