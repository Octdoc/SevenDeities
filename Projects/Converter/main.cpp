#include "cvt_converter.h"

using namespace octdoc;

//INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdline, INT ncmd)
int wmain()
{
	Program::Initialize();
	form::Window::P window = form::Window::Create(L"Converter", 1280, 720);
	form::WindowSettings settings;
	settings.windowName = L"renderScreen";
	settings.hasFrame = false;
	settings.x = 0;
	settings.y = 0;
	settings.width = 800;
	settings.height = 600;
	settings.parentHandle = window->getHWND();
	form::Window::P renderScreen = form::Window::Create(settings);
	renderScreen->InitGraphics();
	renderScreen->setScene(cvt::Converter::Create());
	window->AddChild(renderScreen);
	window->AddChild(form::Button::Create(window, L"btn", 900, 20, 100, 40));
	Program::Instance().SubscribeMessageHandler(renderScreen->getScene());
	Program::Instance().SubscribeAutoUpdater(renderScreen->getScene());
	Program::Instance().RegisterWindow(window);
	window.reset();
	renderScreen.reset();
	Program::Instance().Run(false);
	return 0;
}