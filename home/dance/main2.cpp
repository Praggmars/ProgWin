//#include <windows.h>
//#include <string>
//#include <fstream>
//#include <vector>
//#include <thread>
//#include <chrono>
//
//#define SCREEN_WIDTH 640
//#define SCREEN_HEIGHT 480
//
//const WCHAR appName[] = L"Dance";
//HWND mainWindow;
//
//COLORREF g_screen[SCREEN_HEIGHT][SCREEN_WIDTH];
//
//int g_activeFrame = 0;
//std::vector<std::vector<POINT>> g_frames;
//
//void DrawNextFrame()
//{
//	for (POINT p : g_frames[g_activeFrame])
//		g_screen[p.x][p.y + 100] = 0;
//	g_activeFrame = (g_activeFrame + 1) % g_frames.size();
//	for (POINT p : g_frames[g_activeFrame])
//		g_screen[p.x][p.y + 100] = 0xffffffff;
//	InvalidateRect(mainWindow, NULL, FALSE);
//}
//
//void LoadFramesTXT()
//{
//	POINT p;
//	std::ifstream infile;
//	int counter = 0;
//	infile.open("dance/dance" + std::to_string(++counter) + ".txt", std::ios::in);
//	while (infile.good())
//	{
//		g_frames.push_back(std::vector<POINT>());
//		std::vector<POINT>& points = g_frames[counter - 1];
//		infile >> p.x;
//		infile >> p.y;
//		while (!infile.eof())
//		{
//			if (p.x < 450 || p.y < 380)
//				points.push_back(p);
//			infile >> p.x;
//			infile >> p.y;
//		}
//		infile.close();
//		infile.open("dance/dance" + std::to_string(++counter) + ".txt", std::ios::in);
//	}
//	infile.close();
//}
//void WriteFramesDAT()
//{
//	std::ofstream of;
//	of.open("dance.dat", std::ios::out | std::ios::binary);
//	LONG frameCount = g_frames.size();
//	of.write((char*)&frameCount, sizeof(frameCount));
//	for (std::vector<POINT>& points : g_frames)
//	{
//		LONG wpc = points.size();
//		of.write((char*)&wpc, sizeof(wpc));
//		of.write((char*)points.data(), sizeof(points[0])*wpc);
//	}
//}
//void ReadFramesDAT()
//{
//	std::ifstream infile;
//	infile.open("dance.dat", std::ios::in | std::ios::binary);
//	if (!infile.good())
//		return;
//	LONG frameCount;
//	infile.read((char*)&frameCount, sizeof(frameCount));
//	g_frames.resize(frameCount);
//	for (std::vector<POINT>& points : g_frames)
//	{
//		LONG wpc;
//		infile.read((char*)&wpc, sizeof(wpc));
//		points.resize(wpc);
//		infile.read((char*)points.data(), sizeof(POINT)*wpc);
//	}
//}
//
//void StartRenderingThread()
//{
//	std::thread th([]()->void {
//		auto endtime = std::chrono::system_clock::now() + std::chrono::milliseconds(69);
//		while (true)
//		{
//			DrawNextFrame();
//			while (std::chrono::system_clock::now() < endtime);
//			endtime = std::chrono::system_clock::now() + std::chrono::milliseconds(69);
//		}
//	});
//	th.detach();
//}
//
//LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
//{
//	HDC hdc;
//	HDC hmem;
//	HBITMAP hbmp;
//	PAINTSTRUCT ps;
//	RECT rect;
//
//	switch (msg)
//	{
//	case WM_CREATE:
//		LoadFramesTXT();
//		WriteFramesDAT();
//		//ReadFramesDAT();
//		SetTimer(hwnd, 1, 69, NULL);
//		//StartRenderingThread();
//		return 0;
//
//	case WM_PAINT:
//		hdc = BeginPaint(hwnd, &ps);
//
//		hbmp = CreateBitmap(SCREEN_WIDTH, SCREEN_HEIGHT, 1, 32, g_screen);
//		hmem = CreateCompatibleDC(hdc);
//		SelectObject(hmem, hbmp);
//		BitBlt(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, hmem, 0, 0, SRCCOPY);
//		DeleteObject(hmem);
//
//		EndPaint(hwnd, &ps);
//		return 0;
//
//	case WM_TIMER:
//		if (wparam == 1)
//			DrawNextFrame();
//		return 0;
//
//	case WM_DESTROY:
//		if (mainWindow == hwnd)
//			PostQuitMessage(0);
//		return 0;
//	}
//	return DefWindowProc(hwnd, msg, wparam, lparam);
//}
//
//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR szCMLine, int iCmdShow)
//{
//	WNDCLASS wc;
//	DWORD style, styleEx;;
//	RECT rect;
//	MSG msg;
//
//	wc.style = CS_HREDRAW | CS_VREDRAW;
//	wc.lpfnWndProc = WndProc;
//	wc.cbClsExtra = 0;
//	wc.cbWndExtra = 0;
//	wc.hInstance = hInstance;
//	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
//	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
//	wc.lpszMenuName = NULL;
//	wc.lpszClassName = appName;
//	RegisterClass(&wc);
//
//	rect.left = 0;
//	rect.top = 0;
//	rect.right = SCREEN_WIDTH;
//	rect.bottom = SCREEN_HEIGHT;
//	style = WS_OVERLAPPEDWINDOW & (~(WS_THICKFRAME | WS_MAXIMIZEBOX));
//	styleEx = WS_EX_OVERLAPPEDWINDOW;
//	AdjustWindowRectEx(&rect, style, FALSE, styleEx);
//	mainWindow = CreateWindowEx(styleEx, appName, appName, style,
//		(GetSystemMetrics(SM_CXSCREEN) - rect.right + rect.left) / 2,
//		(GetSystemMetrics(SM_CYSCREEN) - rect.bottom + rect.top) / 2,
//		rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);
//
//	ShowWindow(mainWindow, iCmdShow);
//	UpdateWindow(mainWindow);
//
//	while (GetMessage(&msg, NULL, 0, 0))
//	{
//		TranslateMessage(&msg);
//		DispatchMessage(&msg);
//	}
//
//	return msg.wParam;
//}