#include "test_scene.h"

//INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdline, INT ncmd)
int main()
{
	std::shared_ptr<gfw::Window> window;
	test::Test_Scene scene;
	gfw::GraphicsSettings settings;
	settings.vsync = true;
	settings.windowName = L"Physics Test";
	try
	{
		window = gfw::Window::Create(settings);
		window->setScene(&scene);
		window->Run();
	}
	catch (hcs::Exception& e)
	{
		e.ShowMessageBox();
	}
	return 0;
}