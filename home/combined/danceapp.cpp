#include "danceapp.h"
#include <fstream>

#define ID_TIMER1 1

namespace dnc
{
	void DanceApp::LoadFile()
	{
		std::ifstream infile;
		infile.open("dance.dat", std::ios::in | std::ios::binary);
		if (!infile.good())
			throw std::exception("missing file: dance.dat");
		infile.read((char*)&m_frameCount, sizeof(m_frameCount));
		infile.read((char*)&m_frameRate, sizeof(m_frameRate));
		infile.read((char*)&m_height, sizeof(m_height));
		infile.read((char*)&m_width, sizeof(m_width));
		m_frames.resize(m_frameCount);
		for (std::vector<POINT>& points : m_frames)
		{
			LONG wpc;
			infile.read((char*)&wpc, sizeof(wpc));
			points.resize(wpc);
			infile.read((char*)points.data(), sizeof(POINT)*wpc);
		}
	}
	void DanceApp::CreateScreen()
	{
		m_screen = new COLORREF*[m_height];
		*m_screen = new COLORREF[m_height*m_width];
		memset(*m_screen, 0, sizeof(COLORREF)*m_height*m_width);
		for (LONG i = 1; i < m_height; i++)
			m_screen[i] = &(m_screen[i - 1][m_width]);
	}
	void DanceApp::DeleteScreen()
	{
		delete[] * m_screen;
		delete[] m_screen;
		m_screen = nullptr;
	}
	void DanceApp::DrawFrame()
	{
		for (size_t i = 0; i < m_frames[m_currentFrame].size(); i++)
		{
			POINT p = m_frames[m_currentFrame][i];
			m_screen[p.y][p.x] = 0x00000000;
		}
		m_currentFrame = (m_currentFrame + 1) % m_frameCount;
		for (size_t i = 0; i < m_frames[m_currentFrame].size(); i++)
		{
			POINT p = m_frames[m_currentFrame][i];
			m_screen[p.y][p.x] = 0xffffffff;
		}
	}
	void DanceApp::Paint()
	{
		PAINTSTRUCT ps;
		RECT rect;
		HDC hdc = BeginPaint(m_hwnd, &ps);
		GetClientRect(m_hwnd, &rect);
		HBITMAP hbmp = CreateBitmap(m_width, m_height, 1, 32, *m_screen);
		HDC hmem = CreateCompatibleDC(hdc);
		SelectObject(hmem, hbmp);
		StretchBlt(hdc, 0, 0, rect.right, rect.bottom, hmem, 0, 0, m_width, m_height, SRCCOPY);
		DeleteObject(hmem);
		EndPaint(m_hwnd, &ps);
	}
	DanceApp::DanceApp(HWND hwnd) :AppBase(hwnd)
	{
		LoadFile();
		CreateScreen();
		SetWindowSizeTitle(m_width, m_height, L"Dance");
		m_currentFrame = 0;
		SetTimer(hwnd, ID_TIMER1, m_frameRate, NULL);
	}
	DanceApp::~DanceApp()
	{
		DeleteScreen();
		KillTimer(m_hwnd, ID_TIMER1);
	}
	void DanceApp::MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case WM_TIMER:
			if (LOWORD(wparam) == ID_TIMER1)
			{
				DrawFrame();
				InvalidateRect(hwnd, NULL, FALSE);
			}
			break;
		case WM_PAINT:
			Paint();
			break;
		}
	}
}