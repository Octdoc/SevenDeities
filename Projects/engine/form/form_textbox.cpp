#include "form_textbox.h"

namespace form
{
	TextBox::TextBox(const form::Window::P parent, const WCHAR text[], int x, int y, int w, int h)
	{
		if (text)
			m_windowName = text;
		m_boundingbox.left = x;
		m_boundingbox.top = y;
		m_boundingbox.right = x + w;
		m_boundingbox.bottom = y + h;
		m_hwnd = CreateWindow(L"edit", m_windowName.c_str(), WS_VISIBLE | WS_CHILD, getX(), getY(), getW(), getH(), parent->getHWND(), NULL, GetModuleHandle(NULL), NULL);
	}
	TextBox::P TextBox::Create(const form::Window::P parent, const WCHAR* text)
	{
		return std::make_shared<TextBox>(parent, text, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT);
	}
	TextBox::P TextBox::Create(const form::Window::P parent, int x, int y, const WCHAR* text)
	{
		return std::make_shared<TextBox>(parent, text, x, y, CW_USEDEFAULT, CW_USEDEFAULT);
	}
	TextBox::P TextBox::Create(const form::Window::P parent, int x, int y, int w, int h, const WCHAR* text)
	{
		return std::make_shared<TextBox>(parent, text, x, y, w, h);
	}
}