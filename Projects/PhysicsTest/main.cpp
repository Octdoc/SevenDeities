#include "test_scene.h"
#include <iostream>

using namespace octdoc;

//INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdline, INT ncmd)
int main()
{
	Program::Initialize();
	form::Window::P window = form::Window::Create(L"Physics Test", 1280, 720);
	test::Test_Scene::P scene = test::Test_Scene::Create();
	window->InitGraphics();
	window->setScene(scene);
	Program::Instance().SubscribeMessageHandler(scene);
	Program::Instance().SubscribeAutoUpdater(scene);

	Program::Instance().RegisterWindow(window);
	Program::Instance().Run(true);
	return 0;
}