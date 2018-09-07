#include "test_scene.h"

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdline, INT ncmd)
{
	gfw::Window window;
	test::Test_Scene scene;
	gfw::GraphicsSettings settings;
	settings.windowName = L"Physics Test";
	try
	{
		window.Initialize(settings);
		window.setScene(&scene);
		window.Run();
	}
	catch (hcs::Exception& e)
	{
		e.ShowMessageBox();
	}
	window.Shutdown();
	return 0;
}