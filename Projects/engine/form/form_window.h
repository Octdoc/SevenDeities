#pragma once

#include "form_form.h"

namespace form
{
	struct WindowSettings
	{
		int x;
		int y;
		int width;
		int height;
		bool fullscreen;
		bool resizeable;
		bool hasFrame;
		HWND parentHandle;
		std::wstring windowName;

	public:
		WindowSettings();
	};


	class Scene :public hcs::MessageHandler, public hcs::AutoUpdater
	{
		SHARED_ONLY(Scene);

	protected:
		Scene() = default;
		Form::P m_window;
		gfw::Graphics::P m_graphics;

	public:
		void setWindow(Form::P window);
		virtual void Start() = 0;
	};

	class Window :public Form
	{
		SHARED_ONLY(Window);

	protected:
		gfw::Graphics::P m_graphics;
		Scene::P m_scene;
		bool m_fullscreen;

	private:
		Window(WindowSettings& settings);
		void InitializeWindow(WindowSettings& settings);

		void FillWndClassEx(WNDCLASSEX& wc);
		void FillDevModeSettings(DEVMODE& devMode);
		void FillFullscreenBoundingBox(WindowSettings& settings);
		void FillWindowedBoundingBox(WindowSettings& settings);
		void CreateHWND(WindowSettings& settings);
		void CreateFullscreenWindow(WindowSettings& settings);
		void CreateOverlappedWindow(WindowSettings& settings);
		void ShutdownWindow();

	public:
		~Window();
		virtual void Destroy() override;

		static Window::P Create();
		static Window::P Create(int width, int height);
		static Window::P Create(const WCHAR wndname[], int width, int height);
		static Window::P Create(WindowSettings& settings);

		bool isFullscreen();
		void InitGraphics();
		void InitGraphics(gfw::GraphicsSettings settings);
		gfw::Graphics::P getGraphics();
		void setScene(Scene::P scene);
		Scene::P getScene();
	};
}