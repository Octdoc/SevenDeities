#include "form_button.h"

namespace form
{
	Button::Button(const form::Window::P parent, const WCHAR text[], int x, int y, int w, int h, HMENU id)
	{
		m_windowName = text;
		m_id = id;
		m_boundingbox.left = x;
		m_boundingbox.top = y;
		m_boundingbox.right = x + w;
		m_boundingbox.bottom = y + h;
		Initialize(parent);
	}
	void Button::Initialize(const form::Window::P parent)
	{
		m_hwnd = CreateWindow(L"button", m_windowName.c_str(), WS_VISIBLE | WS_CHILD, getX(), getY(), getW(), getH(), parent->getHWND(), m_id, GetModuleHandle(NULL), NULL);
	}
	bool Button::IDMatch(HMENU id)
	{
		return m_id == id;
	}
	HMENU Button::getID()
	{
		return m_id;
	}
	Button::P Button::Create(const form::Window::P parent, const WCHAR text[], HMENU id)
	{
		return std::make_shared<Button>(parent, text, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, id);
	}
	Button::P Button::Create(const form::Window::P parent, const WCHAR text[], int x, int y, HMENU id)
	{
		return std::make_shared<Button>(parent, text, x, y, CW_USEDEFAULT, CW_USEDEFAULT, id);
	}
	Button::P Button::Create(const form::Window::P parent, const WCHAR text[], int x, int y, int w, int h, HMENU id)
	{
		return std::make_shared<Button>(parent, text, x, y, w, h, id);
	}
}