#pragma once

#include "form_window.h"

namespace form
{
	class Button :public Form
	{
		SHARED_ONLY(Button);
		HMENU m_id;

	private:
		Button(const form::Window::P parent, const WCHAR text[], int x, int y, int w, int h, HMENU id);

		void Initialize(const form::Window::P parent);

	public:
		static Button::P Create(const form::Window::P parent, const WCHAR text[], HMENU id);
		static Button::P Create(const form::Window::P parent, const WCHAR text[], int x, int y, HMENU id);
		static Button::P Create(const form::Window::P parent, const WCHAR text[], int x, int y, int w, int h, HMENU id);
		bool IDMatch(HMENU id);
		HMENU getID();
	};
}