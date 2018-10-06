#include "form_checkbox.h"

namespace form
{
	CheckBox::CheckBox(const form::Window::P parent, const WCHAR text[], int x, int y, int w, int h, HMENU id)
	{
		m_windowName = text;
		m_id = id;
		m_boundingbox.left = x;
		m_boundingbox.top = y;
		m_boundingbox.right = x + w;
		m_boundingbox.bottom = y + h;
		Initialize(parent);
	}
	void CheckBox::Initialize(const form::Window::P parent)
	{
		m_hwnd = CreateWindow(L"button", m_windowName.c_str(), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, getX(), getY(), getW(), getH(), parent->getHWND(), m_id, GetModuleHandle(NULL), NULL);
	}
	bool CheckBox::IDMatch(HMENU id)
	{
		return m_id == id;
	}
	HMENU CheckBox::getID()
	{
		return m_id;
	}
	bool CheckBox::isChecked()
	{
		return false;
	}
	CheckBox::P CheckBox::Create(const form::Window::P parent, const WCHAR text[], HMENU id)
	{
		return std::make_shared<CheckBox>(parent, text, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, id);
	}
	CheckBox::P CheckBox::Create(const form::Window::P parent, const WCHAR text[], int x, int y, HMENU id)
	{
		return std::make_shared<CheckBox>(parent, text, x, y, CW_USEDEFAULT, CW_USEDEFAULT, id);
	}
	CheckBox::P CheckBox::Create(const form::Window::P parent, const WCHAR text[], int x, int y, int w, int h, HMENU id)
	{
		return std::make_shared<CheckBox>(parent, text, x, y, w, h, id);
	}
}