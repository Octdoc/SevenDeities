#include "gfw_window.h"


LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
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
		wc.lpfnWndProc = WindowProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hIconSm = wc.hIcon;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
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
	void Window::FillFullscreenBoundingBox(WindowSettings& settings)
	{
		m_boundingBox.left = 0;
		m_boundingBox.top = 0;
		m_boundingBox.right = settings.width;
		m_boundingBox.bottom = settings.height;
	}
	void Window::FillWindowedBoundingBox(WindowSettings& settings)
	{
		if (settings.x < 0)
			settings.x = (GetSystemMetrics(SM_CXSCREEN) - settings.width) / 2;
		if (settings.y < 0)
			settings.y = (GetSystemMetrics(SM_CYSCREEN) - settings.height) / 2;
		m_boundingBox.left = 0;
		m_boundingBox.top = 0;
		m_boundingBox.right = settings.width;
		m_boundingBox.bottom = settings.height;
		if (settings.hasFrame)
			AdjustWindowRectEx(&m_boundingBox, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);
		m_boundingBox.left += settings.x;
		m_boundingBox.right += settings.x;
		m_boundingBox.top += settings.y;
		m_boundingBox.bottom += settings.y;
	}
	void Window::CreateHWND(WindowSettings& settings, Window::P parent)
	{
		if (m_fullscreen && parent == nullptr)
			CreateFullscreenWindow(settings);
		else
			CreateOverlappedWindow(settings, parent);

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
			, m_boundingBox.left, m_boundingBox.top, GetClientWidth(), GetClientHeight(),
			NULL, NULL, m_hInstance, NULL);
	}
	void Window::CreateOverlappedWindow(WindowSettings& settings, Window::P parent)
	{
		FillWindowedBoundingBox(settings);
		DWORD exStyle = 0;
		DWORD style = 0;
		if (settings.hasFrame)
		{
			exStyle = WS_EX_OVERLAPPEDWINDOW;
			style = WS_OVERLAPPEDWINDOW;
			if (parent)
				style |= WS_CHILD | WS_VISIBLE;
			if (!settings.resizeable)
				style &= (~(WS_THICKFRAME | WS_MAXIMIZEBOX));
		}
		else
		{
			exStyle = WS_EX_APPWINDOW;
			style = parent ? WS_CHILD | WS_VISIBLE : WS_POPUP;
		}

		m_hwnd = CreateWindowEx(exStyle, m_windowName.c_str(), m_windowName.c_str(), style,
			m_boundingBox.left, m_boundingBox.top, GetClientWidth(), GetClientHeight(),
			parent ? parent->getHWND() : NULL, NULL, m_hInstance, NULL);
	}
	void Window::InitializeWindow(WindowSettings& settings, Window::P parent)
	{
		WNDCLASSEX wc;
		m_windowName = settings.windowName;
		m_fullscreen = settings.fullscreen;
		m_hInstance = GetModuleHandle(NULL);
		FillWndClassEx(wc);
		RegisterClassEx(&wc);
		CreateHWND(settings, parent);
		ShowWindow(m_hwnd, SW_SHOW);
	}
	Window::Window(WindowSettings& settings, Window::P parent)
	{
		m_parent = parent;
		InitializeWindow(settings, parent);
	}
	Window::~Window()
	{
		ShutdownWindow();
		UnregisterClass(m_windowName.c_str(), m_hInstance);
	}
	Window::P Window::Create()
	{
		WindowSettings settings;
		return Create(settings);
	}
	Window::P Window::Create(WindowSettings& settings, Window::P parent)
	{
		Window::P window = std::make_shared<Window>(settings, parent);
		window->m_self = window;
		if (parent)
			parent->AddChild(window);
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
		Start(periodicUpdate);
		while (msg.message != WM_QUIT)
			MessageLoop(msg);
	}
	void Window::AddChild(Window::W child)
	{
		m_children.push_back(child);
	}
	void Window::Start(bool periodicUpdate)
	{
		m_periodicUpdate = periodicUpdate;
		m_timer.Reset();
		if (m_scene)
		{
			//m_scene->Start();
			if (!m_periodicUpdate);
			//m_scene->Render();
		}
		for (Window::W child : m_children)
		{
			Window::P c = child.lock();
			c->Start(periodicUpdate);
		}
	}
	void Window::Frame()
	{
		m_timer.Update();
		if (m_scene)
		{
			//m_scene->Update(m_timer.GetTimeDelta(), m_timer.GetTimeTotal());
			//m_scene->Render();
		}
		for (Window::W child : m_children)
		{
			Window::P c = child.lock();
			c->Frame();
		}
	}
	void Window::MessageLoop(MSG& msg)
	{
		if (m_periodicUpdate)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				MessageHandler(msg);
			}
			else
			{
				Frame();
			}
		}
		else
		{
			if (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				MessageHandler(msg);
			}
		}
	}
	void Window::MessageHandler(MSG& msg)
	{
		/*if (m_input)
			m_input->HandleMessage(msg);
		if (m_scene)
			m_scene->MessageHandler(msg);*/
		for (Window::W child : m_children)
		{
			Window::P c = child.lock();
			c->MessageHandler(msg);
		}
	}
	/*void Window::setScene(Scene::P scene)
	{
		m_scene = scene;
		m_scene->SetWindow(m_self.lock());
	}
	Scene::P Window::getScene()
	{
		return m_scene;
	}*/
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
		AddGraphics(settings);
	}
	void Window::AddGraphics(GraphicsSettings& settings)
	{
		if (settings.width < 0)
			settings.width = GetClientWidth();
		if (settings.height < 0)
			settings.height = GetClientHeight();
		settings.fullscreen = m_fullscreen;
		m_graphics = Graphics::Create(m_hwnd, settings);
	}
	Graphics::P Window::getGraphics()
	{
		return m_graphics;
	}
	/*void Window::AddInput()
	{
		m_input = hcs::Input::Create();
	}
	hcs::Input::P Window::getInput()
	{
		return m_input;
	}*/
	HWND Window::getHWND()
	{
		return m_hwnd;
	}
	HINSTANCE Window::getHInstance()
	{
		return m_hInstance;
	}
	bool Window::isFullscreen()
	{
		return m_fullscreen;
	}


	WindowSettings::WindowSettings()
		:x(-1),
		y(-1),
		width(1280),
		height(720),
		fullscreen(false),
		resizeable(false),
		hasFrame(true),
		windowName(L"Octdoc") {}
}