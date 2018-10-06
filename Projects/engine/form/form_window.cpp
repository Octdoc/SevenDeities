#include "form_window.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
void RemoveScene(form::Scene::P scene);

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

	std::map<std::wstring, size_t> Window::m_registeredClasses;

	void Window::RegisterWindowClass(const WCHAR className[])
	{
		auto wcname = m_registeredClasses.find(className);
		if (wcname == m_registeredClasses.end())
		{
			WNDCLASSEX wc{};
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
			wc.lpszClassName = className;
			wc.cbSize = sizeof(WNDCLASSEX);
			RegisterClassEx(&wc);
			m_registeredClasses[className] = 1;
		}
		else
		{
			wcname->second++;
		}
	}
	void Window::UnregisterWindowClass(const WCHAR className[])
	{
		auto wcname = m_registeredClasses.find(className);
		if (--wcname->second == 0)
		{
			UnregisterClass(className, GetModuleHandle(NULL));
			m_registeredClasses.erase(wcname);
		}
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
			, m_windowName.c_str(), m_windowName.c_str(),
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

		m_hwnd = CreateWindowEx(exStyle, m_windowName.c_str(), m_windowName.c_str(), style,
			getX(), getY(), getW(), getH(),
			settings.parentHandle ? settings.parentHandle : NULL, NULL, GetModuleHandle(NULL), NULL);
		GetWindowRect(m_hwnd, &m_boundingbox);
	}
	void Window::InitializeWindow(WindowSettings& settings)
	{
		m_windowName = settings.windowName;
		m_fullscreen = settings.fullscreen;
		RegisterWindowClass(m_windowName.c_str());
		CreateHWND(settings);
		ShowWindow(m_hwnd, SW_SHOW);
	}
	Window::Window(WindowSettings& settings)
	{
		InitializeWindow(settings);
	}
	Window::~Window()
	{
		Destroy();
	}
	void Window::Destroy()
	{
		Form::Destroy();
		ShutdownWindow();
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
		RemoveScene(m_scene);
		UnregisterWindowClass(m_windowName.c_str());
	}
	bool Window::isFullscreen()
	{
		return m_fullscreen;
	}
	void Window::InitGraphics()
	{
		gfw::GraphicsSettings settings;
		InitGraphics(settings);
	}
	void Window::InitGraphics(gfw::GraphicsSettings settings)
	{
		if (settings.width < 0)
			settings.width = getW();
		if (settings.height < 0)
			settings.height = getH();
		m_graphics = gfw::Graphics::Create(m_hwnd, settings);
	}
	Scene::P Window::getScene()
	{
		return m_scene;
	}
	gfw::Graphics::P Window::getGraphics()
	{
		return m_graphics;
	}
	void Window::setScene(Scene::P scene)
	{
		RemoveScene(m_scene);
		m_scene = scene;
		scene->setWindow(m_self.lock());
		scene->Start();
	}
	void Scene::setWindow(Form::P window)
	{
		m_window = window;
		m_graphics = std::dynamic_pointer_cast<Window>(window)->getGraphics();
	}
}