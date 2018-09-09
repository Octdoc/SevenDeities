#include "sd_scene.h"

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdline, INT ncmd)
{
	std::shared_ptr<gfw::Window> window;
	sd::SD_Scene scene;
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
	window->Shutdown();
	return 0;
}