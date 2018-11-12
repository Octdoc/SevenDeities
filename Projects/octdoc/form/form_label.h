#pragma once

#include "form_window.h"

namespace octdoc
{
	namespace form
	{
		class Label :public Form
		{
			SHARED_ONLY(Label)

		private:
			Label(const Window::P parent, const WCHAR text[], int x, int y, int w, int h);

		public:
			static Label::P Create(const Window::P parent, const WCHAR *text = nullptr);
			static Label::P Create(const Window::P parent, int x, int y, const WCHAR *text = nullptr);
			static Label::P Create(const Window::P parent, int x, int y, int w, int h, const WCHAR *text = nullptr);
		};
	}
}