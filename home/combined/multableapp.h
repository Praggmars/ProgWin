#pragma once

#include "appbase.h"

namespace mtbl
{
	class MultableApp :public AppBase
	{
		HWND m_multiplierTrackBar;
		HWND m_moduloTrackBar;
		double m_multiplier;
		int m_modulo;

	private:
		void Paint();

	public:
		MultableApp(HWND hwnd);
		virtual ~MultableApp();
		virtual void MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;
		virtual LPCWSTR HelpDialogText() override;
	};
}