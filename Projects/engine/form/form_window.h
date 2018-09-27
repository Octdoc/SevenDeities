#pragma once

#include "graphics/gfw_scene.h"

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

	class Window :public Form
	{
		SHARED_ONLY(Window);

		bool m_fullscreen;

		gfw::Graphics::P m_graphics;

		gfw::Scene::P m_scene;

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

		static Window::P Create();
		static Window::P Create(int width, int height);
		static Window::P Create(const WCHAR wndname[], int width, int height);
		static Window::P Create(WindowSettings& settings);

		void AddGraphics();
		void AddGraphics(gfw::GraphicsSettings& settings);
		gfw::Graphics::P getGraphics();

		void setScene(gfw::Scene::P scene);
		gfw::Scene::P getScene();
		bool isFullscreen();
	};
}