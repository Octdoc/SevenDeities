#pragma once

#include "gfw_graphics.h"
#include "helpers/hcs_timer.h"
#include "helpers/hcs_input.h"

namespace gfw
{
	class Scene;

	struct WindowSettings
	{
		int width;
		int height;
		bool fullscreen;
		std::wstring windowName;

	public:
		WindowSettings();
	};

	class Window
	{
		SHARED_ONLY(Window);

		Window::W m_self;

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
		Window(int width, int height, bool fullscreen, std::wstring& windowName);
		void Initialize(int width, int height, bool fullscreen, std::wstring& windowName);

		void FillWndClassEx(WNDCLASSEX& wc);
		void FillDevModeSettings(DEVMODE& devMode);
		void FillFullscreenBoundingBox(int width, int height);
		void FillWindowedBoundingBox(int width, int height);
		void CreateHWND(int width, int height);
		void CreateFullscreenWindow(int width, int height);
		void CreateOverlappedWindow(int width, int height);
		void Frame();
		void MessageLoop(MSG& msg);
		void MessageHandler(MSG& msg);
		void ShutdownWindow();

	public:
		~Window();

		static Window::P Create(bool initGraphics = false);
		static Window::P Create(WindowSettings& settings);
		static Window::P Create(GraphicsSettings& settings);

		void Run(bool periodicUpdate = true);

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