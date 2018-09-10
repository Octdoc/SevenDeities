#include "sd_scene.h"

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdline, INT ncmd)
{
	gfw::Window::P window;
	try
	{
		window = gfw::Window::Create();
		window->setScene(sd::SD_Scene::Create());
		window->Run();
	}
	catch (hcs::Exception& e)
	{
		e.ShowMessageBox();
	}
	return 0;
}