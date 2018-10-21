#include "fractalapp.h"

#define ID_TIMER1 1

namespace frac
{
	void FractalApp::DrawNextDot()
	{
		RECT rect;
		m_prevDot = (m_vertices[rand() % m_vertices.size()] - m_prevDot)*(m_vertices.size() - 2) / (m_vertices.size() - 1) + m_prevDot;
		m_dots.push_back(m_prevDot);
		GetClientRect(m_hwnd, &rect);
		int x = X(m_prevDot(0), rect.right);
		int y = Y(m_prevDot(1), rect.bottom);
		HDC hdc = GetDC(m_hwnd);
		Ellipse(hdc, x - (m_dotSize / 2), y - (m_dotSize / 2), x + (m_dotSize / 2), y + (m_dotSize / 2));
		ReleaseDC(m_hwnd, hdc);
	}
	void FractalApp::Paint()
	{
		PAINTSTRUCT ps;
		RECT rect;
		HDC hdc = BeginPaint(m_hwnd, &ps);
		GetClientRect(m_hwnd, &rect);
		for (size_t i = 0; i < m_vertices.size(); i++)
		{
			int x = X(m_vertices[i](0), rect.right);
			int y = Y(m_vertices[i](1), rect.bottom);
			Ellipse(hdc, x - (m_vertexSize / 2), y - (m_vertexSize / 2), x + (m_vertexSize / 2), y + (m_vertexSize / 2));
		}
		for (size_t i = 0; i < m_dots.size(); i++)
		{
			int x = X(m_dots[i](0), rect.right);
			int y = Y(m_dots[i](1), rect.bottom);
			Ellipse(hdc, x - (m_dotSize / 2), y - (m_dotSize / 2), x + (m_dotSize / 2), y + (m_dotSize / 2));
		}
		EndPaint(m_hwnd, &ps);
	}
	void FractalApp::PlaceVertex(int x, int y)
	{
		RECT rect;
		GetClientRect(m_hwnd, &rect);
		m_prevDot(0) = (((double)x / (double)rect.right) * 2.0) - 1.0;
		m_prevDot(1) = (((double)y / (double)rect.bottom) * -2.0) + 1.0;
		m_vertices.push_back(m_prevDot);
		m_dots.clear();
		m_prevDot = 0.0;
		m_dots.push_back(m_prevDot);
		InvalidateRect(m_hwnd, NULL, TRUE);
	}
	void FractalApp::RemoveVertex(int x, int y)
	{
		RECT rect;
		GetClientRect(m_hwnd, &rect);
		linalg::Vector<double, 2> v;
		v(0) = (((double)x / (double)rect.right) * 2.0) - 1.0;
		v(1) = (((double)y / (double)rect.bottom) * -2.0) + 1.0;
		std::vector<linalg::Vector<double, 2>>::iterator best = m_vertices.end();
		double mindst = 0.001;
		for (auto it = m_vertices.begin(); it != m_vertices.end(); it++)
		{
			if ((*it - v).LengthSquare() < mindst)
			{
				mindst = (*it - v).LengthSquare();
				best = it;
			}
		}
		if (best != m_vertices.end())
		{
			m_vertices.erase(best);
			m_dots.clear();
			m_prevDot = 0.0;
			m_dots.push_back(m_prevDot);
			InvalidateRect(m_hwnd, NULL, TRUE);
		}
	}
	FractalApp::FractalApp(HWND hwnd) :AppBase(hwnd)
	{
		m_prevDot(0) = 0.0;
		m_prevDot(1) = 0.9;
		m_vertices.push_back(m_prevDot);
		m_prevDot(0) = -0.9;
		m_prevDot(1) = -0.9;
		m_vertices.push_back(m_prevDot);
		m_prevDot(0) = 0.9;
		m_prevDot(1) = -0.9;
		m_vertices.push_back(m_prevDot);
		m_prevDot(0) = 0.0;
		m_prevDot(1) = 0.0;
		m_dots.push_back(m_prevDot);
		m_dotSize = 5;
		m_vertexSize = 11;
		SetTimer(hwnd, ID_TIMER1, 50, NULL);
		SetWindowTitle(L"Fractal");
		InvalidateRect(hwnd, NULL, TRUE);
	}
	FractalApp::~FractalApp()
	{
		KillTimer(m_hwnd, ID_TIMER1);
	}

	void FractalApp::MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case WM_TIMER:
			if (wparam == ID_TIMER1)
				DrawNextDot();
			break;
		case WM_PAINT:
			Paint();
			break;
		case WM_LBUTTONDOWN:
			PlaceVertex(LOWORD(lparam), HIWORD(lparam));
			break;
		case WM_RBUTTONDOWN:
			RemoveVertex(LOWORD(lparam), HIWORD(lparam));
			break;
		}
	}
}