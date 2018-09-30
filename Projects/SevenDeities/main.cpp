#include "sd_scene.h"

using namespace octdoc;

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdline, INT ncmd)
{
	Program::Initialize();
	form::Window::P window = form::Window::Create(L"Seven Deities", 1280, 720);
	form::WindowSettings settings;
	settings.x = 200;
	settings.y = 160;
	settings.width = 640;
	settings.height = 360;
	settings.windowName = L"Child";
	settings.parentHandle = window->getHWND();
	//settings.hasFrame = false;
	form::Window::P child = form::Window::Create(settings);
	child->InitGraphics();
	child->setScene(sd::SD_Scene::Create());
	window->AddChild(child);
	Program::Instance().SubscribeMessageHandler(child->getScene());
	Program::Instance().SubscribeAutoUpdater(child->getScene());
	Program::Instance().RegisterWindow(window);
	window.reset();
	child.reset();
	Program::Instance().Run(true);
	return 0;
}