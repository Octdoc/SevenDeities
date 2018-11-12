#include "form_listbox.h"

namespace octdoc
{
	namespace form
	{
		ListBox::ListBox(const Window::P parent, int x, int y, int w, int h)
			:Form(parent)
		{
			m_hwnd = CreateWindow(L"listbox", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL, x, y, w, h, parent->getHWND(), NULL, GetModuleHandle(NULL), NULL);
		}
		ListBox::P ListBox::Create(const Window::P parent)
		{
			return Create(parent, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT);
		}
		ListBox::P ListBox::Create(const Window::P parent, int x, int y)
		{
			return Create(parent, CW_USEDEFAULT, CW_USEDEFAULT, x, y);
		}
		ListBox::P ListBox::Create(const Window::P parent, int x, int y, int w, int h)
		{
			return std::make_shared<ListBox>(parent, x, y, w, h);
		}
		void ListBox::AddString(const WCHAR* str)
		{
			SendMessage(m_hwnd, LB_ADDSTRING, 0, (LPARAM)str);
		}
		void ListBox::Clear()
		{
			SendMessage(m_hwnd, LB_RESETCONTENT, 0, 0);
		}
		std::wstring ListBox::GetSelected()
		{
			return std::wstring();
		}
	}
}