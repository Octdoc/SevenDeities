#include "cvt_converter.h"

//INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdline, INT ncmd)
int wmain()
{
	gfw::Window window;
	cvt::Converter scene;
	try
	{
		window.Initialize();
		window.setScene(&scene);
		window.Run(false);
	}
	catch (hcs::Exception& e)
	{
		e.ShowMessageBox();
	}
	window.Shutdown();
	return 0;
}