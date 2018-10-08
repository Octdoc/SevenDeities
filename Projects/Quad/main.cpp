#include "quad_scene.h"

using namespace octdoc;

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdline, INT ncmd)
{
	try
	{
		Program::Initialize();
		form::Window::P window = form::Window::Create(L"Quadruped", 1280, 720);
		form::WindowSettings settings;
		settings.x = 0;
		settings.y = 0;
		settings.width = 1000;
		settings.height = 720;
		settings.windowName = L"Child";
		settings.parentHandle = window->getHWND();
		settings.hasFrame = false;
		form::Window::P child = form::Window::Create(settings);
		child->InitGraphics();
		child->setScene(quad::Scene::Create());
		window->AddChild(child);
		Program::Instance().SubscribeMessageHandler(child->getScene());
		Program::Instance().SubscribeAutoUpdater(child->getScene());
		Program::Instance().RegisterWindow(window);
		window.reset();
		child.reset();
		Program::Instance().Run(true);
	}
	catch (hcs::Exception e)
	{
		e.ShowMessageBox();
	}
	return 0;
}