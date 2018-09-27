#include "cvt_converter.h"

//INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdline, INT ncmd)
int wmain()
{
	gfw::Window::P window;
	gfw::Window::P renderScreen;
	form::Button::P m_button;
	gfw::WindowSettings settings;
	settings.windowName = L"Converter";
	try
	{
		window = gfw::Window::Create(settings);
		m_button = form::Button::Create(window, L"Button");
		settings.windowName = L"renderScreen";
		settings.hasFrame = false;
		settings.x = 100;
		settings.y = 100;
		settings.width = 800;
		settings.height = 6000;
		renderScreen = gfw::Window::Create(settings, window);
		renderScreen->AddGraphics();
		renderScreen->AddInput();
		renderScreen->setScene(cvt::Converter::Create());
		window->Run(false);
	}
	catch (hcs::Exception& e)
	{
		e.ShowMessageBox();
	}
	return 0;
}