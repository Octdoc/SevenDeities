#include "cvt_converter.h"

//INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdline, INT ncmd)
int wmain()
{
	std::shared_ptr<gfw::Window> window;
	cvt::Converter scene;
	try
	{
		window = gfw::Window::Create();
		window->setScene(&scene);
		window->Run();
	}
	catch (hcs::Exception& e)
	{
		e.ShowMessageBox();
	}
	return 0;
}