#pragma once

#include "form_window.h"

namespace form
{
	class CheckBox :public Form
	{
		SHARED_ONLY(CheckBox);
		HMENU m_id;

	private:
		CheckBox(const form::Window::P parent, const WCHAR text[], int x, int y, int w, int h, HMENU id);

		void Initialize(const form::Window::P parent);

	public:
		static CheckBox::P Create(const form::Window::P parent, const WCHAR text[], HMENU id);
		static CheckBox::P Create(const form::Window::P parent, const WCHAR text[], int x, int y, HMENU id);
		static CheckBox::P Create(const form::Window::P parent, const WCHAR text[], int x, int y, int w, int h, HMENU id);
		bool IDMatch(HMENU id);
		HMENU getID();
		bool isChecked();
	};
}