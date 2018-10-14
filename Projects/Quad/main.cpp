#include "quad_scene.h"

using namespace octdoc;

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdline, INT ncmd)
//int main()
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
		window->AddChild(child);
		form::Label::P labels[3];
		window->AddChild(labels[0] = form::Label::Create(window, 1024, 100, 200, 24));
		window->AddChild(labels[1] = form::Label::Create(window, 1024, 130, 200, 24));
		window->AddChild(labels[2] = form::Label::Create(window, 1024, 160, 200, 24));
		child->setScene(quad::Scene::Create(labels));
		Program::Instance().SubscribeMessageHandler(child->getScene());
		Program::Instance().SubscribeAutoUpdater(child->getScene());
		Program::Instance().RegisterWindow(window);
		window.reset();
		child.reset();
		labels[0].reset();
		labels[1].reset();
		labels[2].reset();
		Program::Instance().Run(true);
	}
	catch (hcs::Exception e)
	{
		e.ShowMessageBox();
	}
	return 0;
}