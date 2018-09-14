#pragma once

#include "graphics/gfw_window.h"

namespace form
{
	class Button
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

		void setPosition(int x, int y);
		void setX(int x);
		void setY(int y);
		int getX();
		int getY();
		void setSize(int w, int h);
		void setW(int w);
		void setH(int h);
		int getW();
		int getH();
		void setRect(int x, int y, int w, int h);
	};
}