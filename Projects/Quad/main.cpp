#include "quad_scene.h"

using namespace octdoc;

void RegisterWindows()
{
	form::Window::P window = form::Window::Create(L"Quadruped", 1280, 720);
	form::WindowSettings settings;
	settings.x = 0;
	settings.y = 0;
	settings.width = 1000;
	settings.height = 720;
	settings.windowName = L"Child";
	settings.hasFrame = false;
	form::Window::P child = form::Window::Create(settings, window);
	child->InitGraphics();
	form::Label::P labels[3];
	labels[0] = form::Label::Create(window, 1024, 100, 200, 24);
	labels[1] = form::Label::Create(window, 1024, 130, 200, 24);
	labels[2] = form::Label::Create(window, 1024, 160, 200, 24);
	child->setScene(quad::Scene::Create(labels));
}

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdline, INT ncmd)
{
	try
	{
		Program::Initialize();
		RegisterWindows();
		Program::Instance().Run(true);
	}
	catch (hcs::Exception e)
	{
		e.ShowMessageBox();
	}
	return 0;
}