#include "cvt_menu.h"

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR szCmdline, INT iCmd)
{
	Program::Initialize();
	cvt::Menu::Create();
	Program::Instance().Run(false);
	return 0;
}