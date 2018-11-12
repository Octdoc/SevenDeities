#include "form_button.h"

namespace octdoc
{
	namespace form
	{
		Button::Button(const Window::P parent, const WCHAR text[], int x, int y, int w, int h)
			:Form(parent)
		{
			m_id = (HMENU)m_autoIDdistributor++;
			m_hwnd = CreateWindow(L"button", text, WS_VISIBLE | WS_CHILD, x, y, w, h, parent->getHWND(), m_id, GetModuleHandle(NULL), NULL);
		}
		bool Button::IDMatch(HMENU id)
		{
			return m_id == id;
		}
		HMENU Button::getID()
		{
			return m_id;
		}
		Button::P Button::Create(const Window::P parent, const WCHAR text[])
		{
			return Create(parent, text, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT);
		}
		Button::P Button::Create(const Window::P parent, const WCHAR text[], int x, int y)
		{
			return Create(parent, text, x, y, CW_USEDEFAULT, CW_USEDEFAULT);
		}
		Button::P Button::Create(const Window::P parent, const WCHAR text[], int x, int y, int w, int h)
		{
			Button::P button = std::make_shared<Button>(parent, text, x, y, w, h);
			button->m_parent->AddChild(button);
			button->m_self = button;
			return button;
		}
	}
}