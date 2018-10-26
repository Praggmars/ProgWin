#pragma once

#include "appbase.h"

namespace rpl
{
	class RippleApp :public AppBase
	{
		int m_currentBuffer;
		COLORREF **m_buffers[2];
		LONG m_width;
		LONG m_height;
		double m_damping;

	private:
		void CreateBuffers();
		void DeleteBuffers();
		void StartRipple(int x, int y);
		void DrawFrame();
		void Paint();

	public:
		RippleApp(HWND hwnd);
		virtual ~RippleApp();
		virtual void MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;
		virtual LPCWSTR HelpDialogText() override;
	};
}