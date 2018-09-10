#pragma once

#include "gfw_graphics.h"
#include "helpers/hcs_timer.h"
#include "helpers/hcs_input.h"

namespace gfw
{
	class Scene;

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
		Window();
		Window(GraphicsSettings& settings);
		void Initialize();
		void Initialize(GraphicsSettings& settings);
		
		void FillWndClassEx(WNDCLASSEX& wc);
		void FillDevModeSettings(DEVMODE& devMode, unsigned long width, unsigned long height);
		void FillFullscreenBoundingBox(int width, int height);
		void FillWindowedBoundingBox(GraphicsSettings& settings);
		void CreateHWND(GraphicsSettings& settings);
		void CreateFullscreenWindow(GraphicsSettings& settings);
		void CreateOverlappedWindow(GraphicsSettings& settings);
		void Frame();
		void MessageLoop(MSG& msg);
		void MessageHandler(MSG& msg);
		void InitializeWindow(GraphicsSettings& settings);
		void ShutdownWindow();

	public:
		~Window();

		static Window::P Create();
		static Window::P Create(GraphicsSettings& settings);

		void Run(bool periodicUpdate = true);

		void setScene(std::shared_ptr<Scene> scene);
		std::shared_ptr<Scene> getScene();

		void setPeriodicUpdate(bool update);

		int GetWindowWidth();
		int GetWindowHeight();
		Graphics::P getGraphics();
		hcs::Input::P getInput();
		HWND getHWND();
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