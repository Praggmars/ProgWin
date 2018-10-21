#pragma once

#include "appbase.h"
#include "strf_stringfunction.h"

namespace fnc
{
	class FunctionApp :public AppBase
	{
		struct GraphSettings
		{
			int mx;
			int my;
			double distanceFromMiddle;
			double prevwixau;

			double width;
			double height;
			double middlex;
			double middley;
			double widthInXAxisUnit;

			inline void DefaultSettings()
			{
				mx = 0;
				my = 0;
				middlex = 0.0;
				middley = 0.0;
				widthInXAxisUnit = 4.0;
			}
			inline double Zoom()
			{
				return width / widthInXAxisUnit;
			}
			inline double ToGraphXCoord(int x)
			{
				return (x - width / 2) / Zoom() + middlex;
			}
			inline double ToGraphYCoord(int y)
			{
				return (height / 2 - y) / Zoom() + middley;
			}
			inline int ToScreenXCoordClamp(double x)
			{
				x = (x - middlex) * Zoom() + width / 2;
				if (x > INT16_MAX)
					return INT16_MAX;
				if (x < INT16_MIN)
					return INT16_MIN;
				return (int)x;
			}
			inline int ToScreenYCoordClamp(double y)
			{
				y = (middley - y) * Zoom() + height / 2;
				if (y > INT16_MAX)
					return INT16_MAX;
				if (y < INT16_MIN)
					return INT16_MIN;
				return (int)y;
			}
			inline int ToScreenXCoord(double x)
			{
				return (int)((x - middlex) * Zoom() + width / 2);
			}
			inline int ToScreenYCoord(double y)
			{
				return (int)((middley - y) * Zoom() + height / 2);
			}
		};

		GraphSettings m_graphSettings;
		std::wstring m_functionStr;
		strf::Function m_strFunc;

		HWND m_functionInput;
		HWND m_resetButton;
		HWND m_xPosText;
		HWND m_yPosText;
		HWND m_fxPosText;

	private:
		void FillCoordTextBoxes();
		void DrawFunction(HDC hdc, std::function<double(double)> f);
		void DrawAxises(HDC hdc);
		void LoadFunction();
		void Paint();
		void Ev_Command(HWND hwnd, WPARAM wparam);
		void Ev_MouseWheel(HWND hwnd, WPARAM wparam);
		void Ev_MMouseButton(LPARAM lparam);
		void Ev_RMouseButton(HWND hwnd, LPARAM lparam);
		void Ev_MouseMove(HWND hwnd, WPARAM wparam, LPARAM lparam);

	public:
		FunctionApp(HWND hwnd);
		virtual ~FunctionApp();
		virtual void MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;
	};
}