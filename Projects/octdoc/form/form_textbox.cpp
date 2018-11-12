#include "form_textbox.h"

namespace octdoc
{
	namespace form
	{
		TextBox::TextBox(const Window::P parent, const WCHAR text[], int x, int y, int w, int h)
			:Form(parent)
		{
			m_hwnd = CreateWindow(L"edit", text, WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL, x, y, w, h, parent->getHWND(), NULL, GetModuleHandle(NULL), NULL);
		}
		TextBox::P TextBox::Create(const Window::P parent, const WCHAR* text)
		{
			return Create(parent, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, text);
		}
		TextBox::P TextBox::Create(const Window::P parent, int x, int y, const WCHAR* text)
		{
			return Create(parent, x, y, CW_USEDEFAULT, CW_USEDEFAULT, text);
		}
		TextBox::P TextBox::Create(const Window::P parent, int x, int y, int w, int h, const WCHAR* text)
		{
			TextBox::P textBox = std::make_shared<TextBox>(parent, text, x, y, w, h);
			textBox->m_parent->AddChild(textBox);
			textBox->m_self = textBox;
			return textBox;
		}
	}
}