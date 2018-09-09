#include "gfw_window.h"


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
	case WM_CLOSE:
	case WM_QUIT:
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
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = m_windowName.c_str();
		wc.cbSize = sizeof(WNDCLASSEX);
	}
	void Window::FillDevModeSettings(DEVMODE& devMode, unsigned long width, unsigned long height)
	{
		ClearStruct(devMode);
		devMode.dmSize = sizeof(devMode);
		devMode.dmPelsWidth = width;
		devMode.dmPelsHeight = height;
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
	void Window::FillWindowedBoundingBox(GraphicsSettings& settings)
	{
		int deltax = (GetSystemMetrics(SM_CXSCREEN) - settings.width) / 2;
		int deltay = (GetSystemMetrics(SM_CYSCREEN) - settings.height) / 2;
		m_boundingBox.left = 0;
		m_boundingBox.top = 0;
		m_boundingBox.right = settings.width;
		m_boundingBox.bottom = settings.height;
		AdjustWindowRectEx(&m_boundingBox, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);
		m_boundingBox.left += deltax;
		m_boundingBox.right += deltax;
		m_boundingBox.top += deltay;
		m_boundingBox.bottom += deltay;
	}
	void Window::CreateHWND(GraphicsSettings& settings)
	{
		if (m_fullscreen = settings.fullscreen)
			CreateFullscreenWindow(settings);
		else
			CreateOverlappedWindow(settings);

		if (m_hwnd == NULL)
			throw hcs::Exception(L"Error", L"Could not create window");
	}
	void Window::CreateFullscreenWindow(GraphicsSettings& settings)
	{
		DEVMODE dmScreenSettings;
		FillDevModeSettings(dmScreenSettings, settings.width, settings.height);
		FillFullscreenBoundingBox(settings.width, settings.height);
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		m_hwnd = CreateWindowEx(
			WS_EX_APPWINDOW
			, m_windowName.c_str(), m_windowName.c_str(),
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP
			, m_boundingBox.left, m_boundingBox.top, GetWindowWidth(), GetWindowHeight(),
			NULL, NULL, m_hInstance, NULL);
	}
	void Window::CreateOverlappedWindow(GraphicsSettings& settings)
	{
		FillWindowedBoundingBox(settings);

		m_hwnd = CreateWindowEx(
			WS_EX_OVERLAPPEDWINDOW
			, m_windowName.c_str(), m_windowName.c_str(),
			WS_OVERLAPPEDWINDOW
			, m_boundingBox.left, m_boundingBox.top, GetWindowWidth(), GetWindowHeight(),
			NULL, NULL, m_hInstance, NULL);
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
		m_input.HandleMessage(msg);
		if (m_scene)
			m_scene->MessageHandler(msg);
	}
	Window::Window() :m_hwnd(nullptr), m_hInstance(nullptr), m_scene(nullptr)
	{
		Initialize();
	}
	Window::Window(GraphicsSettings& settings) : m_hwnd(nullptr), m_hInstance(nullptr), m_scene(nullptr)
	{
		Initialize(settings);
	}
	void Window::Shutdown()
	{
		if (m_scene)
			m_scene->Quit();
		m_graphics.reset();
		ShutdownWindow();
		UnregisterClass(m_windowName.c_str(), m_hInstance);
		m_hInstance = NULL;
		m_scene = NULL;
	}
	void Window::ShutdownDeleteScene()
	{
		DeleteScene();
		Shutdown();
	}
	void Window::setScene(Scene* scene)
	{
		m_scene = scene;
		m_scene->SetWindow(this);
	}
	Scene* Window::getScene()
	{
		return m_scene;
	}
	void Window::DeleteScene()
	{
		if (m_scene)
		{
			m_scene->Quit();
			delete m_scene;
		}
	}
	void Window::ChangeScene(Scene* scene, bool deletePrevious)
	{
		m_scene->Quit();
		if (deletePrevious)
			delete m_scene;
		m_scene = scene;
		m_scene->SetWindow(this);
		m_scene->Start();
	}
	std::shared_ptr<Window> Window::Create()
	{
		return std::make_shared<Window>();
	}
	std::shared_ptr<Window> Window::Create(GraphicsSettings& settings)
	{
		return std::make_shared<Window>(settings);
	}
	void Window::Initialize()
	{
		GraphicsSettings settings;
		Initialize(settings);
	}
	void Window::Initialize(GraphicsSettings& settings)
	{
		InitializeWindow(settings);
		m_graphics = Graphics::Create(m_hwnd, settings);
		m_input.Initialize();
	}
	void Window::InitializeWindow(GraphicsSettings& settings)
	{
		WNDCLASSEX wc;
		m_windowName = settings.windowName;
		m_hInstance = GetModuleHandle(NULL);
		FillWndClassEx(wc);
		RegisterClassEx(&wc);
		CreateHWND(settings);
		ShowWindow(m_hwnd, SW_SHOW);
		SetForegroundWindow(m_hwnd);
		SetFocus(m_hwnd);
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
		if (m_scene)
			m_scene->Start();
		m_timer.Reset();
		if (!m_periodicUpdate)
			if (m_scene)
				m_scene->Render();
		while (msg.message != WM_QUIT)
			MessageLoop(msg);
	}
	void Window::setPeriodicUpdate(bool update)
	{
		m_periodicUpdate = update;
	}
	int Window::GetWindowWidth()
	{
		return m_boundingBox.right - m_boundingBox.left;
	}
	int Window::GetWindowHeight()
	{
		return m_boundingBox.bottom - m_boundingBox.top;
	}
	Graphics& Window::getGraphics()
	{
		return *m_graphics;
	}
	hcs::Input& Window::getInput()
	{
		return m_input;
	}
	HWND Window::getHWND()
	{
		return m_hwnd;
	}

	void Scene::SetWindow(Window* window)
	{
		m_window = window;
		m_graphics = &window->getGraphics();
		m_input = &window->getInput();
	}
}