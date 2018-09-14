#include "form_button.h"

namespace form
{
	void Button::ApplyWindowSize()
	{
		MoveWindow(m_hwnd, getX(), getY(), getW(), getH(), TRUE);
	}

	Button::Button(const gfw::Window::P parent, const WCHAR text[]) :m_text(text)
	{
		m_boundingbox.top = 100;
		m_boundingbox.bottom = 200;
		m_boundingbox.left = 100;
		m_boundingbox.right = 1100;
		Initialize(parent);
	}
	void Button::Initialize(const gfw::Window::P parent)
	{
		m_hwnd = CreateWindow(L"button", m_text.c_str(), WS_VISIBLE | WS_CHILD, getX(), getY(), getW(), getH(), parent->getHWND(), NULL, NULL, NULL);
	}
	Button::P Button::Create(const gfw::Window::P parent, const WCHAR text[])
	{
		return std::make_shared<Button>(parent, text);
	}

#pragma region getter, setter

	void Button::setPosition(int x, int y)
	{
		m_boundingbox.right += x - m_boundingbox.left;
		m_boundingbox.left = x;
		m_boundingbox.bottom += y - m_boundingbox.top;
		m_boundingbox.top = y;
		ApplyWindowSize();
	}
	void Button::setX(int x)
	{
		m_boundingbox.right += x - m_boundingbox.left;
		m_boundingbox.left = x;
		ApplyWindowSize();
	}
	void Button::setY(int y)
	{
		m_boundingbox.bottom += y - m_boundingbox.top;
		m_boundingbox.top = y;
		ApplyWindowSize();
	}
	int Button::getX()
	{
		return m_boundingbox.left;
	}
	int Button::getY()
	{
		return m_boundingbox.top;
	}
	void Button::setSize(int w, int h)
	{
		m_boundingbox.right = m_boundingbox.left + w;
		m_boundingbox.bottom = m_boundingbox.top + h;
		ApplyWindowSize();
	}
	void Button::setW(int w)
	{
		m_boundingbox.right = m_boundingbox.left + w;
		ApplyWindowSize();
	}
	void Button::setH(int h)
	{
		m_boundingbox.bottom = m_boundingbox.top + h;
		ApplyWindowSize();
	}
	int Button::getW()
	{
		return m_boundingbox.right - m_boundingbox.left;
	}
	int Button::getH()
	{
		return m_boundingbox.bottom - m_boundingbox.top;
	}
	void Button::setRect(int x, int y, int w, int h)
	{
		m_boundingbox.right = x + w;
		m_boundingbox.left = x;
		m_boundingbox.bottom = y + w;
		m_boundingbox.top = y;
		ApplyWindowSize();
	}

#pragma endregion

}