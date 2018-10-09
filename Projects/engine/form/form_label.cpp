#include "form_label.h"

namespace form
{
	Label::Label(const form::Window::P parent, const WCHAR text[], int x, int y, int w, int h)
	{
		if (text)
			m_windowName = text;
		m_boundingbox.left = x;
		m_boundingbox.top = y;
		m_boundingbox.right = x + w;
		m_boundingbox.bottom = y + h;
		m_hwnd = CreateWindow(L"static", m_windowName.c_str(), WS_VISIBLE | WS_CHILD, getX(), getY(), getW(), getH(), parent->getHWND(), NULL, GetModuleHandle(NULL), NULL);
	}
	Label::P Label::Create(const form::Window::P parent, const WCHAR* text)
	{
		return std::make_shared<Label>(parent, text, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT);
	}
	Label::P Label::Create(const form::Window::P parent, int x, int y, const WCHAR* text)
	{
		return std::make_shared<Label>(parent, text, x, y, CW_USEDEFAULT, CW_USEDEFAULT);
	}
	Label::P Label::Create(const form::Window::P parent, int x, int y, int w, int h, const WCHAR* text)
	{
		return std::make_shared<Label>(parent, text, x, y, w, h);
	}
}