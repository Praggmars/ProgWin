#pragma once

#include "appbase.h"
#include <array>

namespace mdb
{
	class MandelbrotApp :public AppBase
	{
		double m_centerX;
		double m_centerY;
		double m_offsetX;
		double m_offsetY;
		double m_zoom;
		LONG m_maxIter;
		COLORREF **m_screen;
		LONG m_width;
		LONG m_height;

	private:
		inline double X(int x) { return (double)(x - m_width / 2) * m_zoom + m_centerX; }
		inline double Y(int y) { return (double)(y - m_height / 2) * m_zoom + m_centerY; }
		void CreateScreen();
		void DeleteScreen();
		COLORREF IterToColor(LONG iter);
		void Paint();
		void DrawMandelbrot();

	public:
		MandelbrotApp(HWND hwnd);
		virtual ~MandelbrotApp();
		virtual void MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;
		virtual LPCWSTR HelpDialogText() override;
	};
}