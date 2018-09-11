#include "form_button.h"

namespace form
{
	void Button::ApplyWindowSize()
	{
		SetWindowPos(m_hwnd, (HWND)HWND_TOP, getX(), getY(), getW(), getH(), 0);
	}
	Button::P Button::Create(const WCHAR text[])
	{
		return std::make_shared<Button>(text);
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
		m_boundingbox.left = m_boundingbox.right + w;
		m_boundingbox.bottom = m_boundingbox.top + h;
		ApplyWindowSize();
	}
	void Button::setW(int w)
	{
		m_boundingbox.left = m_boundingbox.right + w;
		ApplyWindowSize();
	}
	void Button::setH(int h)
	{
		m_boundingbox.bottom = m_boundingbox.top + h;
		ApplyWindowSize();
	}
	int Button::getW()
	{
		return m_boundingbox.bottom - m_boundingbox.top;
	}
	int Button::getH()
	{
		return m_boundingbox.left - m_boundingbox.right;
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