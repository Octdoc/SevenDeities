#pragma once

#include "graphics/gfw_window.h"
#include "form_form.h"

namespace form
{
	class Button :public Form
	{
		SHARED_ONLY(Button);
		HWND m_hwnd;
		std::wstring m_text;
		RECT m_boundingbox;

	private:
		Button(const gfw::Window::P parent, const WCHAR text[]);

		void Initialize(const gfw::Window::P parent);

		void ApplyWindowSize();

	public:
		static Button::P Create(const gfw::Window::P parent, const WCHAR text[]);

	};
}