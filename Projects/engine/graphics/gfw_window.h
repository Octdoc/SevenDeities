#pragma once

#include "gfw_graphics.h"
#include "helpers/hcs_timer.h"
#include "helpers/hcs_input.h"

namespace gfw
{
	class Scene;

	class Window
	{
		RECT m_boundingBox;
		bool m_fullscreen;
		bool m_periodicUpdate;
		std::wstring m_windowName;

		HWND m_hwnd;
		HINSTANCE m_hInstance;

		hcs::Timer m_timer;

		hcs::Input m_input;
		Graphics m_graphics;

		Scene *m_scene;

	private:
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
		Window();

		void Initialize();
		void Initialize(GraphicsSettings& settings);
		void Run(bool periodicUpdate = true);
		void Shutdown();
		void ShutdownDeleteScene();

		void ChangeScene(Scene* scene, bool deletePrevious);
		void setScene(Scene* scene);
		Scene* getScene();
		void DeleteScene();

		void setPeriodicUpdate(bool update);

		int GetWindowWidth();
		int GetWindowHeight();
		Graphics& getGraphics();
		hcs::Input& getInput();
		HWND getHWND();
	};

	class Scene
	{
	protected:
		Window* m_window;
		Graphics* m_graphics;
		hcs::Input* m_input;

	public:
		void SetWindow(Window* window);

		virtual void Start() = 0;
		virtual void Quit() = 0;
		virtual void Update(double deltaTime, double totalTime) = 0;
		virtual void Render() = 0;
		virtual void MessageHandler(MSG& message) = 0;
	};
}