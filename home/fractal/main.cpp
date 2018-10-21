#include "linalg.h"
#include <windows.h>
#include <string>
#include <iostream>
#include <array>
#include <vector>

HWND g_mainWindow;

std::vector<linalg::Vector<double, 2>> vertices;
std::vector<linalg::Vector<double, 2>> dots;
linalg::Vector<double, 2> prevDot;


inline int X(double x, int w)
{
	return (int)((x + 1) / 2 * w);
}
inline int Y(double y, int h)
{
	return (int)((y - 1) / (-2) * h);
}

void CalcNext()
{
	int p = rand() * 100 / RAND_MAX;
	if (p <= 2)
	{
		prevDot(0) = 0;
		prevDot(1) = 0.16*prevDot(1);
	}
	else {
		if (p <= 86) {
			prevDot(0) = 0.85*prevDot(0) + 0.04*prevDot(1);
			prevDot(1) = -0.04*prevDot(0) + 0.85*prevDot(1) + 1.6;
		}
		else {
			if (p <= 93) {
				prevDot(0) = 0.2*prevDot(0) - 0.26*prevDot(1);
				prevDot(1) = 0.23*prevDot(0) + 0.22*prevDot(1) + 1.6;
			}
			else {
				prevDot(0) = -0.15*prevDot(0) + 0.28*prevDot(1);
				prevDot(1) = 0.26*prevDot(0) + 0.24*prevDot(1) + 0.44;
			}
		}
	}
	dots.push_back(prevDot);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	int dotSize = 5;
	int vertexSize = 11;

	switch (msg)
	{
	case WM_TIMER:
		if (wparam == 1)
		{
			prevDot = (vertices[rand() % vertices.size()] - prevDot)*(vertices.size() - 2) / (vertices.size() - 1) + prevDot;
			//CalcNext();
			dots.push_back(prevDot);
			GetClientRect(hwnd, &rect);
			int x = X(prevDot(0), rect.right);
			int y = Y(prevDot(1), rect.bottom);
			hdc = GetDC(hwnd);
			Ellipse(hdc, x - (dotSize / 2), y - (dotSize / 2), x + (dotSize / 2), y + (dotSize / 2));
			ReleaseDC(hwnd, hdc);
		}
		return 0;
	case WM_CREATE:
		prevDot(0) = 0.0;
		prevDot(1) = 0.9;
		vertices.push_back(prevDot);
		prevDot(0) = -0.9;
		prevDot(1) = -0.9;
		vertices.push_back(prevDot);
		prevDot(0) = 0.9;
		prevDot(1) = -0.9;
		vertices.push_back(prevDot);
		prevDot(0) = 0.0;
		prevDot(1) = 0.0;
		dots.push_back(prevDot);
		SetTimer(hwnd, 1, 50, NULL);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		for (size_t i = 0; i < vertices.size(); i++)
		{
			int x = X(vertices[i](0), rect.right);
			int y = Y(vertices[i](1), rect.bottom);
			Ellipse(hdc, x - (vertexSize / 2), y - (vertexSize / 2), x + (vertexSize / 2), y + (vertexSize / 2));
		}
		for (size_t i = 0; i < dots.size(); i++)
		{
			int x = X(dots[i](0), rect.right);
			int y = Y(dots[i](1), rect.bottom);
			Ellipse(hdc, x - (dotSize / 2), y - (dotSize / 2), x + (dotSize / 2), y + (dotSize / 2));
		}
		EndPaint(hwnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
	{
		GetClientRect(hwnd, &rect);
		prevDot(0) = (((double)LOWORD(lparam) / (double)rect.right) * 2.0) - 1.0;
		prevDot(1) = (((double)HIWORD(lparam) / (double)rect.bottom) * -2.0) + 1.0;
		int x = X(prevDot(0), rect.right);
		int y = Y(prevDot(1), rect.bottom);
		vertices.push_back(prevDot);
		dots.clear();
		prevDot = 0.0;
		dots.push_back(prevDot);
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		GetClientRect(hwnd, &rect);
		linalg::Vector<double, 2> v;
		v(0) = (((double)LOWORD(lparam) / (double)rect.right) * 2.0) - 1.0;
		v(1) = (((double)HIWORD(lparam) / (double)rect.bottom) * -2.0) + 1.0;
		int x = X(v(0), rect.right);
		int y = Y(v(1), rect.bottom);
		std::vector<linalg::Vector<double, 2>>::iterator best = vertices.end();
		double mindst = 0.001;
		for (auto it = vertices.begin(); it != vertices.end(); it++)
		{
			if ((*it - v).LengthSquare() < mindst)
			{
				mindst = (*it - v).LengthSquare();
				best = it;
			}
		}
		if (best != vertices.end())
		{
			vertices.erase(best);
			dots.clear();
			prevDot = 0.0;
			dots.push_back(prevDot);
			InvalidateRect(hwnd, NULL, TRUE);
		}
		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR szCMLine, int iCmdShow)
{
	const WCHAR appName[] = L"FractApp";
	WNDCLASS wc;
	MSG msg;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = appName;

	RegisterClass(&wc);
	g_mainWindow = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, appName, L"Fractal", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, GetModuleHandle(NULL), NULL);

	ShowWindow(g_mainWindow, SW_SHOW);
	UpdateWindow(g_mainWindow);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}