#pragma once

#include "form_window.h"

namespace octdoc
{
	namespace form
	{
		class ListBox :public Form
		{
			SHARED_ONLY(ListBox)

		private:
			ListBox(const Window::P parent, int x, int y, int w, int h);

		public:
			static ListBox::P Create(const Window::P parent);
			static ListBox::P Create(const Window::P parent, int x, int y);
			static ListBox::P Create(const Window::P parent, int x, int y, int w, int h);

			void AddString(const WCHAR* str);
			void Clear();
			std::wstring GetSelected();
		};
	}
}