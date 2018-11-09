#include "octdoc.h"

namespace octdoc
{
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
			windowName(L"Octdoc") {}

		std::map<std::wstring, size_t> Window::m_registeredClasses;

		void Window::RegisterWindowClass(const WCHAR className[])
		{
			auto wcname = m_registeredClasses.find(className);
			if (wcname == m_registeredClasses.end())
			{
				WNDCLASSEX wc{};
				wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
				wc.lpfnWndProc = octdoc::Program::WndProc;
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
		void Window::CreateHWND(WindowSettings& settings, HWND parentHandle)
		{
			if (m_fullscreen && parentHandle == nullptr)
				CreateFullscreenWindow(settings);
			else
				CreateOverlappedWindow(settings, parentHandle);

			if (m_hwnd == nullptr)
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
		void Window::CreateOverlappedWindow(WindowSettings& settings, HWND parentHandle)
		{
			FillWindowedBoundingBox(settings);
			DWORD exStyle = 0;
			DWORD style = 0;
			if (settings.hasFrame)
			{
				exStyle = WS_EX_OVERLAPPEDWINDOW;
				style = WS_OVERLAPPEDWINDOW;
				if (parentHandle)
					style |= WS_CHILD | WS_VISIBLE;
				if (!settings.resizeable)
					style &= (~(WS_THICKFRAME | WS_MAXIMIZEBOX));
			}
			else
			{
				exStyle = WS_EX_APPWINDOW;
				style = parentHandle ? WS_CHILD | WS_VISIBLE : WS_POPUP;
			}

			m_hwnd = CreateWindowEx(exStyle, m_windowName.c_str(), m_windowName.c_str(), style,
				getX(), getY(), getW(), getH(),
				parentHandle ? parentHandle : NULL, NULL, GetModuleHandle(NULL), NULL);
			GetWindowRect(m_hwnd, &m_boundingbox);
		}
		void Window::InitializeWindow(WindowSettings& settings, HWND parentHandle)
		{
			m_windowName = settings.windowName;
			m_fullscreen = settings.fullscreen;
			RegisterWindowClass(m_windowName.c_str());
			CreateHWND(settings, parentHandle);
			ShowWindow(m_hwnd, SW_SHOW);
		}
		Window::Window(WindowSettings& settings, FormContainer::P parent)
			:Form(parent ? parent : octdoc::Program::InstancePtr())
		{
			InitializeWindow(settings, (parent ? parent->ToForm()->getHWND() : nullptr));
		}
		Window::~Window()
		{
			if (m_fullscreen)
				ChangeDisplaySettings(NULL, 0);
			UnregisterWindowClass(m_windowName.c_str());
		}
		Window::P Window::Create(FormContainer::P parent)
		{
			WindowSettings settings;
			return Create(settings);
		}
		Window::P Window::Create(int width, int height, FormContainer::P parent)
		{
			WindowSettings settings;
			settings.width = width;
			settings.height = height;
			return Create(settings, parent);
		}
		Window::P Window::Create(const WCHAR wndname[], int width, int height, FormContainer::P parent)
		{
			WindowSettings settings;
			settings.width = width;
			settings.height = height;
			settings.windowName = wndname;
			return Create(settings, parent);
		}
		Window::P Window::Create(WindowSettings& settings, FormContainer::P parent)
		{
			Window::P window = std::make_shared<Window>(settings, parent);
			window->m_parent->AddChild(window);
			window->m_self = window;
			return window;
		}
		bool Window::isFullscreen()
		{
			return m_fullscreen;
		}
		void Window::InitGraphics()
		{
			gfx::GraphicsSettings settings;
			InitGraphics(settings);
		}
		void Window::InitGraphics(gfx::GraphicsSettings& settings)
		{
			if (settings.width < 0)
				settings.width = getW();
			if (settings.height < 0)
				settings.height = getH();
			m_graphics = gfx::Graphics::Create(m_hwnd, settings);
		}
		Scene::P Window::getScene()
		{
			return m_scene;
		}
		void Window::Close()
		{
			Form::Close();
			m_scene.reset();
		}
		void Window::MessageHandler(UINT msg, WPARAM wparam, LPARAM lparam)
		{
			if (msg == WM_DESTROY)
				Close();
			if (m_scene)
				m_scene->MessageHandler(octdoc::Program::Instance().Input());
		}
		void Window::Frame(float deltaTime)
		{
			if (m_scene)
				m_scene->Frame(deltaTime);
		}
		gfx::Graphics::P Window::getGraphics()
		{
			return m_graphics;
		}
		void Window::setScene(Scene::P scene)
		{
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
}