#pragma once

#include "form_window.h"

namespace form
{
	class CheckBox :public Form
	{
		SHARED_ONLY(CheckBox);
		HMENU m_id;

	private:
		CheckBox(const form::Window::P parent, const WCHAR text[], int x, int y, int w, int h);

		void Initialize(const form::Window::P parent);

	public:
		static CheckBox::P Create(const form::Window::P parent, const WCHAR text[]);
		static CheckBox::P Create(const form::Window::P parent, const WCHAR text[], int x, int y);
		static CheckBox::P Create(const form::Window::P parent, const WCHAR text[], int x, int y, int w, int h);
		bool IDMatch(HMENU id);
		HMENU getID();
		bool isChecked();
	};
}