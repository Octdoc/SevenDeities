#pragma once

#include "form_window.h"

namespace octdoc
{
	namespace form
	{
		class TextBox :public Form
		{
			SHARED_ONLY(TextBox)

		private:
			TextBox(const Window::P parent, const WCHAR text[], int x, int y, int w, int h);

		public:
			static TextBox::P Create(const Window::P parent, const WCHAR *text = nullptr);
			static TextBox::P Create(const Window::P parent, int x, int y, const WCHAR *text = nullptr);
			static TextBox::P Create(const Window::P parent, int x, int y, int w, int h, const WCHAR *text = nullptr);
		};
	}
}