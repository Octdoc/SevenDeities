#include "sd_scene.h"

using namespace octdoc;

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdline, INT ncmd)
{
	Program::Initialize();
	form::Window::P window = form::Window::Create(L"Parent", 1280, 720);
	form::WindowSettings settings;
	settings.width = 640;
	settings.height = 480;
	settings.windowName = L"Child";
	settings.parentHandle = window->getHWND();
	window->AddChild(form::Window::Create(settings));
	window->AddGraphics();
	//window->setScene(sd::SD_Scene::Create());

	Program::Instance().RegisterWindow(window);
	Program::Instance().Run();
	return 0;
	/*gfw::Window::P window;
	gfw::Window::P childwindow;
	gfw::WindowSettings settings;
	settings.windowName = L"octchild";
	settings.hasFrame = true;
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
	return 0;*/
}