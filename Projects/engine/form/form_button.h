#pragma once

#include "form_window.h"

namespace form
{
	class Button :public Form
	{
		SHARED_ONLY(Button);
		HWND m_hwnd;
		std::wstring m_text;
		RECT m_boundingbox;

	private:
		Button(const form::Window::P parent, const WCHAR text[]);

		void Initialize(const form::Window::P parent);

		void ApplyWindowSize();

	public:
		static Button::P Create(const form::Window::P parent, const WCHAR text[]);

	};
}