#include "test.h"
#include "genmath.h"
#include <ctime>
#include <chrono>
#include <thread>
#include <future>
#include <mutex>
#include <functional>
#include <forward_list>
#include <Windows.h>
#include <windowsx.h>

std::mutex m;

template<math::uint N>
void f1()
{
	srand(time(NULL));
	math::Matrix<double, N, N> m1;
	for (math::uint x = 0; x < N; x++)
		for (math::uint y = 0; y < N; y++)
			m1(x, y) = (((double)rand() / RAND_MAX) - 0.5)*10.0;

	auto start = std::chrono::steady_clock::now();
	math::Matrix<double, N, N> m2 = m1.Inverse();
	auto end = std::chrono::steady_clock::now();

	math::Matrix<double, N, N> m3 = m1 * m2;

	m.lock();
	std::cout << std::endl << N << ": Process took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds.";
	std::cout << std::endl << N << ": Process took " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds.";
	std::cout << std::endl << N << ": Process took " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " nanoseconds." << std::endl;

	//m1.Print();
	//m2.Print();
	m3.Print();
	m.unlock();
}

using d3 = math::Vector<double, 3>;

bool isPointOnTriangle(d3 tri[], d3 p)
{
	d3 vecs[3] = {tri[1] - tri[0],
		tri[2] - tri[0],
		p - tri[0] };
	double dot00 = vecs[0].Dot(vecs[0]);
	double dot01 = vecs[0].Dot(vecs[1]);
	double dot02 = vecs[0].Dot(vecs[2]);
	double dot11 = vecs[1].Dot(vecs[1]);
	double dot12 = vecs[1].Dot(vecs[2]);
	double denom = dot00 * dot11 - dot01 * dot01;
	double u = (dot11 * dot02 - dot01 * dot12) / denom;
	double v = (dot00 * dot12 - dot01 * dot02) / denom;
	return u >= 0.0f && v >= 0.0f && u + v <= 1.0f;
}

int start();
int wmain()
{
	return start();

	d3 tri[3];
	tri[0](0) = 0.0;
	tri[0](1) = 0.0;
	tri[0](2) = 0.0;
	tri[1](0) = 1.0;
	tri[1](1) = 0.0;
	tri[1](2) = 0.0;
	tri[2](0) = 0.0;
	tri[2](1) = 0.0;
	tri[2](2) = 1.0;
	d3 p;

	p(0) = 0.4;
	p(1) = 0.45;
	p(2) = 0.5;
	std::cout << isPointOnTriangle(tri, p) << std::endl;

	p(0) = 0.6;
	p(1) = 0.0;
	p(2) = 0.5;
	std::cout << isPointOnTriangle(tri, p) << std::endl;

	p(0) = -0.5;
	p(1) = 0.0;
	p(2) = 0.2;
	std::cout << isPointOnTriangle(tri, p) << std::endl;

	system("pause");
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	POINT points[3];
	points[0].x = 100;
	points[0].y = 100;
	points[1].x = 200;
	points[1].y = 300;
	points[2].x = 400;
	points[2].y = 200;
	d3 tri[3];
	tri[0](0) = 10.0;
	tri[0](1) = 10.0;
	tri[0](2) = 0.0;
	tri[1](0) = 20.0;
	tri[1](1) = 30.0;
	tri[1](2) = 0.0;
	tri[2](0) = 40.0;
	tri[2](1) = 20.0;
	tri[2](2) = 0.0;
	d3 p;
	static HBRUSH hb = CreateSolidBrush(0xff0000);

	switch (msg)
	{	
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SelectObject(hdc, hb);
		Polygon(hdc, points, 3);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_MOUSEMOVE:
		p(0) = GET_X_LPARAM(lparam) / 10.0;
		p(1) = GET_Y_LPARAM(lparam) / 10.0;
		p(2) = 2.1;
		DeleteObject(hb);
		hb = CreateSolidBrush(isPointOnTriangle(tri, p) ? 0x00ff00 : 0x0000ff);
		InvalidateRect(hwnd, NULL, FALSE);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

int start()
{
	const WCHAR appName[] = L"GraphApp";
	WNDCLASS wc;
	MSG msg;
	HWND hwnd;

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
	hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, appName, L"Grapher", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, GetModuleHandle(NULL), NULL);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}