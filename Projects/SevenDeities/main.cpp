#include "sd_scene.h"

using namespace octdoc;

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdline, INT ncmd)
{
	Program::Initialize();
	form::Window::P window = form::Window::Create(L"Seven Deities", 1280, 768);
	window->AddChild(form::CheckBox::Create(window, L"PRESS", 1030, 20, 60, 24, NULL));
	form::WindowSettings settings;
	settings.x = 0;
	settings.y = 0;
	settings.width = 1024;
	settings.height = 768;
	settings.windowName = L"Child";
	settings.parentHandle = window->getHWND();
	settings.hasFrame = false;
	form::Window::P child = form::Window::Create(settings);
	child->InitGraphics();
	child->setScene(car::Scene::Create());
	window->AddChild(child);
	Program::Instance().SubscribeMessageHandler(child->getScene());
	Program::Instance().SubscribeAutoUpdater(child->getScene());
	Program::Instance().RegisterWindow(window);
	window.reset();
	child.reset();
	Program::Instance().Run(true);
	return 0;
}