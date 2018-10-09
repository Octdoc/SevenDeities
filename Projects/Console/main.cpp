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

#include "octdoc.h"
#pragma comment(lib, "engine.lib")

std::mutex m;

template<math::uint N>
void f1()
{
	srand((unsigned int)time(NULL));
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
	d3 vecs[3] = { tri[1] - tri[0],
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

class Quad
{
	float m_ox = 0.35f;
	float m_oy = 0.21f;
	float m_oz = 0.4f;
	float m_o1 = 0.18f;
	float m_o2 = 0.4f;
	float m_o3 = 0.8f;
	float m_o3x = 0.04f;
	float m_a1 = mth::pi*0.25f;
	float m_a2 = 0.0f;
	float m_a3 = mth::pi*0.25f;

	bool m_valid[2];
	mth::float3 m_joints[2];
	mth::float3 m_position;

public:
	void ForwardGeometry(mth::float3 joints)
	{
		float cos1 = cosf(m_a1 + joints.x);
		float cos2 = cosf(m_a2 + joints.y);
		float cos23 = cosf(m_a2 + joints.y + m_a3 + joints.z);
		float sin1 = sinf(m_a1 + joints.x);
		float sin2 = sinf(m_a2 + joints.y);
		float sin23 = sinf(m_a2 + joints.y + m_a3 + joints.z);

		m_position.x = cos1 * m_o3x + sin1 * cos23*m_o3 + sin1 * cos2*m_o2 + sin1 * m_o1 + m_ox;
		m_position.y = -sin23 * m_o3 - sin2 * m_o2 + m_oy;
		m_position.z = -sin1 * m_o3x + cos1 * cos23*m_o3 + cos1 * cos2*m_o2 + cos1 * m_o1 + m_oz;

		m_joints[0] = joints;
		m_valid[0] = true;
		m_valid[1] = false;
	}

	void InverseGeometry(mth::float3 position)
	{
		float a, b, d, discriminant, drt, sol;
		m_valid[0] = false;
		m_valid[1] = false;
		m_position = position;

		//first
		a = m_oz - m_position.z;
		b = m_position.x - m_ox;
		d = m_o3x;
		discriminant = a * a + b * b - d * d;
		if (discriminant < 0.0f)
			return;
		drt = sqrtf(discriminant);
		sol = atan2f((a*d + b * drt), (b * d - a * drt));
		if (sol < m_a1 - mth::pi*0.5f || sol > m_a1 + mth::pi*0.5f)
		{
			sol = atan2f((a*d - b * drt), (b * d + a * drt));
			if (sol < m_a1 - mth::pi*0.5f || sol > m_a1 + mth::pi*0.5f)
				return;
		}
		m_joints[0].x = sol;
		m_joints[1].x = sol;

		//second
		a = m_oy - m_position.y;
		b = (m_position.x - m_ox - cosf(sol)*m_o3x) / sinf(sol) - m_o1;
		d = (b*b + a * a + m_o2 * m_o2 - m_o3 * m_o3) / (2.0f*m_o2);
		discriminant = a * a + b * b - d * d;
		if (discriminant < 0.0f)
			return;
		drt = sqrtf(discriminant);
		m_joints[0].y = atan2f((a*d + b * drt), (b * d - a * drt));
		m_joints[1].y = atan2f((a*d - b * drt), (b * d + a * drt));
		m_valid[0] = (m_joints[0].y >= m_a2 - mth::pi*0.5f &&  m_joints[0].y <= m_a2 + mth::pi*0.5f);
		m_valid[1] = (m_joints[1].y >= m_a2 - mth::pi*0.5f &&  m_joints[1].y <= m_a2 + mth::pi*0.5f);

		if (!m_valid[0] && !m_valid[1])
			return;

		//third
		if (m_valid[0])
		{
			m_joints[0].z = atan2f(m_oy - m_position.y - sinf(m_joints[0].y)*m_o2, b - cosf(m_joints[0].y)*m_o2) - m_joints[0].y;
			m_joints[0].x -= m_a1;
			m_joints[0].y -= m_a2;
			m_joints[0].z -= m_a3;
			m_valid[0] = (m_joints[0].z >= m_a3 - mth::pi*0.5f) && (m_joints[0].z <= m_a3 + mth::pi*0.5f);
		}
		if (m_valid[1])
		{
			m_joints[1].z = atan2f(m_oy - m_position.y - sinf(m_joints[1].y)*m_o2, b - cosf(m_joints[1].y)*m_o2) - m_joints[1].y;
			m_joints[1].x -= m_a1;
			m_joints[1].y -= m_a2;
			m_joints[1].z -= m_a3;
			m_valid[1] = (m_joints[1].z >= m_a3 - mth::pi*0.5f) && (m_joints[1].z <= m_a3 + mth::pi*0.5f);
		}
	}

	bool isValid(UINT index)
	{
		return m_valid[index];
	}
	int SolutionCount()
	{
		int count = 0;
		for (int i = 0; i < 2; i++)
			if (m_valid[i])
				count++;
		return count;
	}
	mth::float3 getJoints(UINT index)
	{
		return m_joints[index];
	}
	mth::float3 getPosition()
	{
		return m_position;
	}
};

void QuadFunction(mth::float3 in)
{
	Quad quad;
	quad.ForwardGeometry(in);
	mth::float3 position = quad.getPosition();
	mth::float3 joints[8];
	bool valid[8];
	quad.InverseGeometry(position);
	std::wcout << in.x << std::endl;
	std::wcout << in.y << std::endl;
	std::wcout << in.z << std::endl;
	std::wcout << position.x << std::endl;
	std::wcout << position.y << std::endl;
	std::wcout << position.z << std::endl << std::endl;
	std::wcout << L"Solution count: " << quad.SolutionCount() << std::endl << std::endl;
	for (int i = 0; i < 2; i++)
		if (valid[i] = quad.isValid(i))
			joints[i] = quad.getJoints(i);
	for (int i = 0; i < 2; i++)
	{
		if (valid[i])
		{
			Quad q;
			q.ForwardGeometry(joints[i]);
			mth::float3 pos = q.getPosition();
			std::wcout << L"Solution " << i << L": " << std::endl;
			std::wcout << joints[i].x - in.x << std::endl;
			std::wcout << joints[i].y - in.y << std::endl;
			std::wcout << joints[i].z - in.z << std::endl;
			std::wcout << position.x << std::endl;
			std::wcout << position.y << std::endl;
			std::wcout << position.z << std::endl << std::endl;
		}
	}
}

int wmain()
{
	//return start();
	std::wcout << fmod(9.3, 4.5) << std::endl;

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

	return (int)msg.wParam;
}