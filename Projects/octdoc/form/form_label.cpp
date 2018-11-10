#include "form_label.h"

namespace octdoc
{
	namespace form
	{
		Label::Label(const form::Window::P parent, const WCHAR text[], int x, int y, int w, int h)
			:Form(parent)
		{
			m_hwnd = CreateWindow(L"static", text, WS_VISIBLE | WS_CHILD, x, y, w, h, parent->getHWND(), NULL, GetModuleHandle(NULL), NULL);
		}
		Label::P Label::Create(const form::Window::P parent, const WCHAR* text)
		{
			return Create(parent, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, text);
		}
		Label::P Label::Create(const form::Window::P parent, int x, int y, const WCHAR* text)
		{
			return Create(parent, x, y, CW_USEDEFAULT, CW_USEDEFAULT, text);
		}
		Label::P Label::Create(const form::Window::P parent, int x, int y, int w, int h, const WCHAR* text)
		{
			Label::P label = std::make_shared<Label>(parent, text, x, y, w, h);
			label->m_parent->AddChild(label);
			label->m_self = label;
			return label;
		}
	}
}