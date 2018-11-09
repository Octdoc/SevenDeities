#pragma once

#include "form_form.h"
#include <map>

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
		std::wstring windowName;

	public:
		WindowSettings();
	};


	class Scene
	{
		SHARED_ONLY(Scene);

	protected:
		Scene() = default;
		Form::P m_window;
		gfw::Graphics::P m_graphics;

	public:
		void setWindow(Form::P window);
		virtual void Start() = 0;
		virtual void MessageHandler(hcs::Input& input) = 0;
		virtual void Frame(float deltaTime) = 0;
	};

	class Window :public Form
	{
		SHARED_ONLY(Window);

		static std::map<std::wstring, size_t> m_registeredClasses;

	protected:
		gfw::Graphics::P m_graphics;
		Scene::P m_scene;
		bool m_fullscreen;

	private:
		Window(WindowSettings& settings, FormContainer::P parent);
		void InitializeWindow(WindowSettings& settings, HWND parentHandle);
		static void RegisterWindowClass(const WCHAR className[]);
		static void UnregisterWindowClass(const WCHAR className[]);

		void FillDevModeSettings(DEVMODE& devMode);
		void FillFullscreenBoundingBox(WindowSettings& settings);
		void FillWindowedBoundingBox(WindowSettings& settings);
		void CreateHWND(WindowSettings& settings, HWND parentHandle);
		void CreateFullscreenWindow(WindowSettings& settings);
		void CreateOverlappedWindow(WindowSettings& settings, HWND parentHandle);

	public:
		~Window();
		static Window::P Create(FormContainer::P parent = nullptr);
		static Window::P Create(int width, int height, FormContainer::P parent = nullptr);
		static Window::P Create(const WCHAR wndname[], int width, int height, FormContainer::P parent = nullptr);
		static Window::P Create(WindowSettings& settings, FormContainer::P parent = nullptr);

		bool isFullscreen();
		void InitGraphics();
		void InitGraphics(gfw::GraphicsSettings& settings);
		gfw::Graphics::P getGraphics();
		void setScene(Scene::P scene);
		Scene::P getScene();
		virtual void Close() override;
		virtual void MessageHandler(UINT msg, WPARAM wparam, LPARAM lparam) override;
		virtual void Frame(float deltaTime) override;
	};
}