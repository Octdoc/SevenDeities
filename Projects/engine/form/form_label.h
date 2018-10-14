#pragma once

#include "form_window.h"

namespace form
{
	class Label :public Form
	{
		SHARED_ONLY(Label);

	private:
		Label(const form::Window::P parent, const WCHAR text[], int x, int y, int w, int h);

	public:
		static Label::P Create(const form::Window::P parent, const WCHAR *text = nullptr);
		static Label::P Create(const form::Window::P parent, int x, int y, const WCHAR *text = nullptr);
		static Label::P Create(const form::Window::P parent, int x, int y, int w, int h, const WCHAR *text = nullptr);
	};
}