#include "test_scene.h"

//INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdline, INT ncmd)
int main()
{
	gfw::Window::P window;
	gfw::GraphicsSettings settings;
	settings.vsync = true;
	settings.windowName = L"Physics Test";
	try
	{
		window = gfw::Window::Create(settings);
		window->setScene(test::Test_Scene::Create());
		window->Run();
	}
	catch (hcs::Exception& e)
	{
		e.ShowMessageBox();
	}
	return 0;
}