#include "form_button.h"

namespace form
{

	Button::Button(const form::Window::P parent, const WCHAR text[]) :m_text(text)
	{
		m_boundingbox.top = 100;
		m_boundingbox.bottom = 200;
		m_boundingbox.left = 100;
		m_boundingbox.right = 1100;
		Initialize(parent);
	}
	void Button::Initialize(const form::Window::P parent)
	{
		m_hwnd = CreateWindow(L"button", m_text.c_str(), WS_VISIBLE | WS_CHILD, getX(), getY(), getW(), getH(), parent->getHWND(), NULL, NULL, NULL);
	}
	Button::P Button::Create(const form::Window::P parent, const WCHAR text[])
	{
		return std::make_shared<Button>(parent, text);
	}


}