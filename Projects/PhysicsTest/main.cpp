#include "test_scene.h"
#include <iostream>

//INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdline, INT ncmd)
int main()
{
	gfw::Window::P window;
	gfw::WindowSettings wsettings;
	gfw::GraphicsSettings gsettings;
	wsettings.hasFrame = false;
	gsettings.vsync = true;
	wsettings.windowName = L"Physics Test";
	try
	{
		window = gfw::Window::Create();
		window->AddGraphics();
		window->AddInput();
		window->setScene(test::Test_Scene::Create());
		window->Run();
	}
	catch (hcs::Exception& e)
	{
		e.ShowMessageBox();
	}
	return 0;
}