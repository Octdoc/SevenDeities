#pragma once

#include "form_window.h"

namespace octdoc
{
	namespace form
	{
		class Button :public Form
		{
			SHARED_ONLY(Button)
			HMENU m_id;

		private:
			Button(const form::Window::P parent, const WCHAR text[], int x, int y, int w, int h);

		public:
			static Button::P Create(const form::Window::P parent, const WCHAR text[]);
			static Button::P Create(const form::Window::P parent, const WCHAR text[], int x, int y);
			static Button::P Create(const form::Window::P parent, const WCHAR text[], int x, int y, int w, int h);
			bool IDMatch(HMENU id);
			HMENU getID();
		};
	}
}