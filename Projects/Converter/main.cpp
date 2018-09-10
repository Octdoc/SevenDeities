#include "cvt_converter.h"

//INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdline, INT ncmd)
int wmain()
{
	gfw::Window::P window;
	try
	{
		window = gfw::Window::Create();
		window->setScene(cvt::Converter::Create());
		window->Run();
	}
	catch (hcs::Exception& e)
	{
		e.ShowMessageBox();
	}
	return 0;
}