#include "sd_scene.h"

using namespace octdoc;

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdline, INT ncmd)
{
	Program::Initialize();
	form::Window::P window = form::Window::Create(L"Seven Deities", 1280, 768);
	window->AddChild(form::CheckBox::Create(window, L"PRESS", 1030, 20, 60, 24));
	form::WindowSettings settings;
	settings.x = 40;
	settings.y = 40;
	settings.width = 800;
	settings.height = 600;
	settings.windowName = L"Child";
	settings.hasFrame = true;
	form::Window::P child = form::Window::Create(settings, window);
	child->InitGraphics();
	child->setScene(sd::Scene::Create());
	window.reset();
	child.reset();
	Program::Instance().Run(true);
	return 0;
}