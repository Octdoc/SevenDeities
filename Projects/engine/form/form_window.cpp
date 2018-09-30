#include "form_window.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

namespace form
{
	WindowSettings::WindowSettings()
		:x(-1),
		y(-1),
		width(1280),
		height(720),
		fullscreen(false),
		resizeable(false),
		hasFrame(true),
		parentHandle(nullptr),
		windowName(L"Octdoc") {}
	void Window::FillWndClassEx(WNDCLASSEX& wc)
	{
		ClearStruct(wc);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetModuleHandle(NULL);
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hIconSm = wc.hIcon;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = windowName.c_str();
		wc.cbSize = sizeof(WNDCLASSEX);
	}
	void Window::FillDevModeSettings(DEVMODE& devMode)
	{
		ClearStruct(devMode);
		devMode.dmSize = sizeof(devMode);
		devMode.dmPelsWidth = getW();
		devMode.dmPelsHeight = getH();
		devMode.dmBitsPerPel = 32;
		devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	}
	void Window::FillFullscreenBoundingBox(WindowSettings& settings)
	{
		m_boundingbox.left = 0;
		m_boundingbox.top = 0;
		m_boundingbox.right = settings.width;
		m_boundingbox.bottom = settings.height;
	}
	void Window::FillWindowedBoundingBox(WindowSettings& settings)
	{
		if (settings.x < 0)
			settings.x = (GetSystemMetrics(SM_CXSCREEN) - settings.width) / 2;
		if (settings.y < 0)
			settings.y = (GetSystemMetrics(SM_CYSCREEN) - settings.height) / 2;
		m_boundingbox.left = 0;
		m_boundingbox.top = 0;
		m_boundingbox.right = settings.width;
		m_boundingbox.bottom = settings.height;
		if (settings.hasFrame)
			AdjustWindowRectEx(&m_boundingbox, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);
		m_boundingbox.left += settings.x;
		m_boundingbox.right += settings.x;
		m_boundingbox.top += settings.y;
		m_boundingbox.bottom += settings.y;
	}
	void Window::CreateHWND(WindowSettings& settings)
	{
		if (m_fullscreen && settings.parentHandle == nullptr)
			CreateFullscreenWindow(settings);
		else
			CreateOverlappedWindow(settings);

		if (m_hwnd == NULL)
			throw hcs::Exception(L"Error", L"Could not create window");
	}
	void Window::CreateFullscreenWindow(WindowSettings& settings)
	{
		FillFullscreenBoundingBox(settings);

		DEVMODE dmScreenSettings;
		FillDevModeSettings(dmScreenSettings);
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		m_hwnd = CreateWindowEx(
			WS_EX_APPWINDOW
			, windowName.c_str(), windowName.c_str(),
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP
			, getX(), getY(), getW(), getH(),
			NULL, NULL, GetModuleHandle(NULL), NULL);
	}
	void Window::CreateOverlappedWindow(WindowSettings& settings)
	{
		FillWindowedBoundingBox(settings);
		DWORD exStyle = 0;
		DWORD style = 0;
		if (settings.hasFrame)
		{
			exStyle = WS_EX_OVERLAPPEDWINDOW;
			style = WS_OVERLAPPEDWINDOW;
			if (settings.parentHandle)
				style |= WS_CHILD | WS_VISIBLE;
			if (!settings.resizeable)
				style &= (~(WS_THICKFRAME | WS_MAXIMIZEBOX));
		}
		else
		{
			exStyle = WS_EX_APPWINDOW;
			style = settings.parentHandle ? WS_CHILD | WS_VISIBLE : WS_POPUP;
		}

		m_hwnd = CreateWindowEx(exStyle, windowName.c_str(), windowName.c_str(), style,
			CW_USEDEFAULT, CW_USEDEFAULT, getW(), getH(),
			settings.parentHandle ? settings.parentHandle : NULL, NULL, GetModuleHandle(NULL), NULL);
		GetWindowRect(m_hwnd, &m_boundingbox);
	}
	void Window::InitializeWindow(WindowSettings& settings)
	{
		WNDCLASSEX wc;
		windowName = settings.windowName;
		m_fullscreen = settings.fullscreen;
		FillWndClassEx(wc);
		RegisterClassEx(&wc);
		CreateHWND(settings);
		ShowWindow(m_hwnd, SW_SHOW);
	}
	Window::Window(WindowSettings& settings)
	{
		InitializeWindow(settings);
	}
	Window::~Window()
	{
		ShutdownWindow();
		UnregisterClass(windowName.c_str(), GetModuleHandle(NULL));
	}
	Window::P Window::Create()
	{
		WindowSettings settings;
		return Create(settings);
	}
	Window::P Window::Create(int width, int height)
	{
		WindowSettings settings;
		settings.width = width;
		settings.height = height;
		return Create(settings);
	}
	Window::P Window::Create(const WCHAR wndname[], int width, int height)
	{
		WindowSettings settings;
		settings.width = width;
		settings.height = height;
		settings.windowName = wndname;
		return Create(settings);
	}
	Window::P Window::Create(WindowSettings& settings)
	{
		Window::P window = std::make_shared<Window>(settings);
		window->m_self = window;
		return window;
	}
	void Window::ShutdownWindow()
	{
		if (m_fullscreen)
			ChangeDisplaySettings(NULL, 0);
		if (m_hwnd != NULL)
		{
			DestroyWindow(m_hwnd);
			m_hwnd = NULL;
		}
	}
	void Window::setScene(gfw::Scene::P scene)
	{
		m_scene = scene;
		m_scene->setWndGfx(m_self.lock(), m_graphics);
		m_scene->Start();
	}
	gfw::Scene::P Window::getScene()
	{
		return m_scene;
	}
	void Window::AddGraphics()
	{
		gfw::GraphicsSettings settings;
		AddGraphics(settings);
	}
	void Window::AddGraphics(gfw::GraphicsSettings& settings)
	{
		if (settings.width < 0)
			settings.width = getW();
		if (settings.height < 0)
			settings.height = getH();
		settings.fullscreen = m_fullscreen;
		m_graphics = gfw::Graphics::Create(m_hwnd, settings);
	}
	gfw::Graphics::P Window::getGraphics()
	{
		return m_graphics;
	}
	bool Window::isFullscreen()
	{
		return m_fullscreen;
	}
}