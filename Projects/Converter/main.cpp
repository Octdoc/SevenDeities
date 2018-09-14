#include "cvt_converter.h"

//INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdline, INT ncmd)
int wmain()
{
	gfw::Window::P window;
	gfw::WindowSettings settings;
	settings.windowName = L"Converter";
	try
	{
		window = gfw::Window::Create(settings);
		window->AddGraphics();
		window->AddInput();
		window->setScene(cvt::Converter::Create());
		window->Run(false);
	}
	catch (hcs::Exception& e)
	{
		e.ShowMessageBox();
	}
	return 0;
}