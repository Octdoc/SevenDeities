#include "form_checkbox.h"

namespace octdoc
{
	namespace form
	{
		CheckBox::CheckBox(const form::Window::P parent, const WCHAR text[], int x, int y, int w, int h)
			:Form(parent)
		{
			m_id = (HMENU)m_autoIDdistributor++;
			m_hwnd = CreateWindow(L"button",text, WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, x, y, w, h, parent->getHWND(), m_id, GetModuleHandle(NULL), NULL);
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
			return SendMessage(m_hwnd, BM_GETCHECK, (WPARAM)0, (LPARAM)0) == BST_CHECKED;
		}
		void CheckBox::setCheck(bool check)
		{
			PostMessage(m_hwnd, BM_SETCHECK, check ? BST_CHECKED : BST_UNCHECKED, LPARAM(0));
		}
		CheckBox::P CheckBox::Create(const form::Window::P parent, const WCHAR text[])
		{
			return Create(parent, text, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT);
		}
		CheckBox::P CheckBox::Create(const form::Window::P parent, const WCHAR text[], int x, int y)
		{
			return Create(parent, text, x, y, CW_USEDEFAULT, CW_USEDEFAULT);
		}
		CheckBox::P CheckBox::Create(const form::Window::P parent, const WCHAR text[], int x, int y, int w, int h)
		{
			CheckBox::P checkBox = std::make_shared<CheckBox>(parent, text, x, y, w, h);
			checkBox->m_parent->AddChild(checkBox);
			checkBox->m_self = checkBox;
			return checkBox;
		}
	}
}