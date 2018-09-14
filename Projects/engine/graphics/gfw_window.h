#pragma once

#include "gfw_graphics.h"
#include "helpers/hcs_timer.h"
#include "helpers/hcs_input.h"

namespace gfw
{
	class Scene;

	struct WindowSettings
	{
		int x;
		int y;
		int width;
		int height;
		bool fullscreen;
		bool resizeable;
		bool hasFrame;
		std::wstring windowName;

	public:
		WindowSettings();
	};

	class Window
	{
		SHARED_ONLY(Window);

		Window::W m_self;
		Window::W m_parent;
		std::vector<Window::W> m_children;

		RECT m_boundingBox;
		bool m_fullscreen;
		bool m_periodicUpdate;
		std::wstring m_windowName;

		HWND m_hwnd;
		HINSTANCE m_hInstance;

		hcs::Timer m_timer;

		hcs::Input::P m_input;
		Graphics::P m_graphics;

		std::shared_ptr<Scene> m_scene;

	private:
		Window(WindowSettings& settings, Window::P parent);
		void InitializeWindow(WindowSettings& settings, Window::P parent);

		void FillWndClassEx(WNDCLASSEX& wc);
		void FillDevModeSettings(DEVMODE& devMode);
		void FillFullscreenBoundingBox(WindowSettings& settings);
		void FillWindowedBoundingBox(WindowSettings& settings);
		void CreateHWND(WindowSettings& settings, Window::P parent);
		void CreateFullscreenWindow(WindowSettings& settings);
		void CreateOverlappedWindow(WindowSettings& settings, Window::P parent);

		void Start(bool periodicUpdate);
		void Frame();
		void MessageLoop(MSG& msg);
		void MessageHandler(MSG& msg);
		void ShutdownWindow();

	public:
		~Window();

		static Window::P Create();
		static Window::P Create(WindowSettings& settings, Window::P parent = nullptr);

		void Run(bool periodicUpdate = true);

		void AddChild(Window::W child);

		void AddGraphics();
		void AddGraphics(GraphicsSettings& settings);
		Graphics::P getGraphics();

		void AddInput();
		hcs::Input::P getInput();

		void setScene(std::shared_ptr<Scene> scene);
		std::shared_ptr<Scene> getScene();

		void setPeriodicUpdate(bool update);

		int GetClientWidth();
		int GetClientHeight();
		HWND getHWND();
		HINSTANCE getHInstance();
		bool isFullscreen();
	};

	class Scene
	{
		SHARED_ONLY(Scene);

	protected:
		Scene() = default;
		Window::P m_window;
		Graphics::P m_graphics;
		hcs::Input::P m_input;

	public:
		void SetWindow(Window::P window);

		virtual void Start() = 0;
		virtual void Quit() = 0;
		virtual void Update(double deltaTime, double totalTime) = 0;
		virtual void Render() = 0;
		virtual void MessageHandler(MSG& message) = 0;
	};
}