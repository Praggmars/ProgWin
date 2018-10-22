#pragma once

#include "appbase.h"
#include <vector>

namespace dnc
{
	class DanceApp :public AppBase
	{
		COLORREF** m_screen;
		std::vector<std::vector<POINT>> m_frames;
		LONG m_currentFrame;
		LONG m_frameCount;
		LONG m_frameRate;
		LONG m_width;
		LONG m_height;

	private:
		void LoadFile();
		void CreateScreen();
		void DeleteScreen();
		void DrawFrame();
		void Paint();

	public:
		DanceApp(HWND hwnd);
		virtual ~DanceApp();
		virtual void MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;
		virtual LPCWSTR HelpDialogText() override;
	};

}