#include "gfw_window.h"


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}

namespace gfw
{
	void Window::FillWndClassEx(WNDCLASSEX& wc)
	{
		ClearStruct(wc);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hIconSm = wc.hIcon;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = m_windowName.c_str();
		wc.cbSize = sizeof(WNDCLASSEX);
	}
	void Window::FillDevModeSettings(DEVMODE& devMode)
	{
		ClearStruct(devMode);
		devMode.dmSize = sizeof(devMode);
		devMode.dmPelsWidth = GetClientWidth();
		devMode.dmPelsHeight = GetClientHeight();
		devMode.dmBitsPerPel = 32;
		devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	}
	void Window::FillFullscreenBoundingBox(int width, int height)
	{
		m_boundingBox.left = 0;
		m_boundingBox.top = 0;
		m_boundingBox.right = width;
		m_boundingBox.bottom = height;
	}
	void Window::FillWindowedBoundingBox(int width, int height)
	{
		int deltax = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		int deltay = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
		m_boundingBox.left = 0;
		m_boundingBox.top = 0;
		m_boundingBox.right = width;
		m_boundingBox.bottom = height;
		AdjustWindowRectEx(&m_boundingBox, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);
		m_boundingBox.left += deltax;
		m_boundingBox.right += deltax;
		m_boundingBox.top += deltay;
		m_boundingBox.bottom += deltay;
	}
	void Window::CreateHWND(int width, int height)
	{
		if (m_fullscreen)
			CreateFullscreenWindow(width, height);
		else
			CreateOverlappedWindow(width, height);

		if (m_hwnd == NULL)
			throw hcs::Exception(L"Error", L"Could not create window");
	}
	void Window::CreateFullscreenWindow(int width, int height)
	{
		FillFullscreenBoundingBox(width, height);

		DEVMODE dmScreenSettings;
		FillDevModeSettings(dmScreenSettings);
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		m_hwnd = CreateWindowEx(
			WS_EX_APPWINDOW
			, m_windowName.c_str(), m_windowName.c_str(),
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP
			, m_boundingBox.left, m_boundingBox.top, GetClientWidth(), GetClientHeight(),
			NULL, NULL, m_hInstance, NULL);
	}
	void Window::CreateOverlappedWindow(int width, int height)
	{
		FillWindowedBoundingBox(width, height);

		m_hwnd = CreateWindowEx(
			WS_EX_OVERLAPPEDWINDOW
			, m_windowName.c_str(), m_windowName.c_str(),
			WS_OVERLAPPEDWINDOW
			, m_boundingBox.left, m_boundingBox.top, GetClientWidth(), GetClientHeight(),
			NULL, NULL, m_hInstance, NULL);
	}
	void Window::Initialize(int width, int height, bool fullscreen, std::wstring& windowName)
	{
		WNDCLASSEX wc;
		m_windowName = windowName;
		m_fullscreen = fullscreen;
		m_hInstance = GetModuleHandle(NULL);
		FillWndClassEx(wc);
		RegisterClassEx(&wc);
		CreateHWND(width, height);
		ShowWindow(m_hwnd, SW_SHOW);
		SetForegroundWindow(m_hwnd);
		SetFocus(m_hwnd);
	}
	Window::Window(int width, int height, bool fullscreen, std::wstring& windowName) : m_hwnd(nullptr), m_hInstance(nullptr), m_scene(nullptr)
	{
		Initialize(width, height, fullscreen, windowName);
	}
	Window::~Window()
	{
		ShutdownWindow();
		UnregisterClass(m_windowName.c_str(), m_hInstance);
	}
	Window::P Window::Create(bool initGraphics)
	{
		if (initGraphics)
		{
			GraphicsSettings settings;
			return Create(settings);
		}
		WindowSettings settings;
		return Create(settings);
	}
	Window::P Window::Create(WindowSettings& settings)
	{
		Window::P window = std::make_shared<Window>(settings.width, settings.height, settings.fullscreen, settings.windowName);
		window->m_self = window;
		return window;
	}
	Window::P Window::Create(GraphicsSettings& settings)
	{
		Window::P window = std::make_shared<Window>(settings.width, settings.height, settings.fullscreen, settings.windowName);
		window->m_self = window;
		window->AddInput();
		window->AddGraphics(settings);
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
	void Window::Run(bool periodicUpdate)
	{
		MSG msg;
		msg.message = WM_NULL;
		m_periodicUpdate = periodicUpdate;
		m_timer.Reset();
		if (m_scene)
		{
			m_scene->Start();
			if (!m_periodicUpdate)
				m_scene->Render();
		}
		while (msg.message != WM_QUIT)
			MessageLoop(msg);
	}
	void Window::Frame()
	{
		m_timer.Update();
		if (m_scene)
		{
			m_scene->Update(m_timer.GetTimeDelta(), m_timer.GetTimeTotal());
			m_scene->Render();
		}
	}
	void Window::MessageLoop(MSG& msg)
	{
		if (m_periodicUpdate)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				MessageHandler(msg);
			else
				Frame();
		}
		else
		{
			if (GetMessage(&msg, NULL, 0, 0))
				MessageHandler(msg);
		}
	}
	void Window::MessageHandler(MSG& msg)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (m_input)
			m_input->HandleMessage(msg);
		if (m_scene)
			m_scene->MessageHandler(msg);
	}
	void Window::setScene(Scene::P scene)
	{
		m_scene = scene;
		m_scene->SetWindow(m_self.lock());
	}
	Scene::P Window::getScene()
	{
		return m_scene;
	}
	void Window::setPeriodicUpdate(bool update)
	{
		m_periodicUpdate = update;
	}
	int Window::GetClientWidth()
	{
		return m_boundingBox.right - m_boundingBox.left;
	}
	int Window::GetClientHeight()
	{
		return m_boundingBox.bottom - m_boundingBox.top;
	}
	void Window::AddGraphics()
	{
		GraphicsSettings settings;
		m_graphics = Graphics::Create(m_hwnd, settings);
	}
	void Window::AddGraphics(GraphicsSettings& settings)
	{
		m_graphics = Graphics::Create(m_hwnd, settings);
	}
	Graphics::P Window::getGraphics()
	{
		return m_graphics;
	}
	void Window::AddInput()
	{
		m_input = hcs::Input::Create();
	}
	hcs::Input::P Window::getInput()
	{
		return m_input;
	}
	HWND Window::getHWND()
	{
		return m_hwnd;
	}
	HINSTANCE Window::getHInstance()
	{
		return m_hInstance;
	}	
	void Scene::SetWindow(Window::P window)
	{
		m_window = window;
		m_graphics = window->getGraphics();
		m_input = window->getInput();
	}

	WindowSettings::WindowSettings()
		:width(1280),
		height(720),
		fullscreen(false),
		windowName(L"Octdoc") {}
}