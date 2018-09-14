#include "sd_scene.h"

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdline, INT ncmd)
{
	gfw::Window::P window;
	gfw::Window::P childwindow;
	gfw::WindowSettings settings;
	settings.windowName = L"octchild";
	settings.x = 100;
	settings.y = 100;
	settings.width = 800;
	settings.height = 450;
	form::Button::P button;
	try
	{
		window = gfw::Window::Create();
		childwindow = gfw::Window::Create(settings, window);
		childwindow->AddGraphics();
		childwindow->AddInput();
		childwindow->setScene(sd::SD_Scene::Create());
		button = form::Button::Create(window, L"Press");
		window->Run();
	}
	catch (hcs::Exception& e)
	{
		e.ShowMessageBox();
	}
	return 0;
}