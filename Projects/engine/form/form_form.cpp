#include "form_form.h"

namespace form
{
	size_t Form::m_autoIDdistributor = 1;

	Form::Form()
	{
		m_hwnd = nullptr;
		ClearStruct(m_boundingbox);
	}
	Form::~Form()
	{
		Destroy();
	}
	void Form::AddChild(Form::P child)
	{
		m_children.push_front(child);
		child->setParent(m_self.lock());
	}
	void Form::RemoveChild(Form::P child)
	{
		m_children.remove(child);
	}
	void Form::RemoveAllChild()
	{
		m_children.clear();
	}
	bool Form::CloseWindow(HWND hwnd)
	{
		if (m_hwnd == hwnd)
		{
			Destroy();
			return true;
		}
		for (auto child : m_children)
			if (child->CloseWindow(hwnd))
				return true;
		return false;
	}
	void Form::Destroy()
	{
		auto child = m_children.begin();
		while (child != m_children.end())
		{
			auto tmp = child++;
			(*tmp)->Destroy();
		}
		if (!m_self.expired())
			m_parent->RemoveChild(m_self.lock());
	}
	void Form::ApplyWindowSize()
	{
		MoveWindow(m_hwnd, getX(), getY(), getW(), getH(), FALSE);
	}

#pragma region getter, setter

	void Form::setPosition(int x, int y)
	{
		m_boundingbox.right += x - m_boundingbox.left;
		m_boundingbox.left = x;
		m_boundingbox.bottom += y - m_boundingbox.top;
		m_boundingbox.top = y;
		ApplyWindowSize();
	}
	void Form::setX(int x)
	{
		m_boundingbox.right += x - m_boundingbox.left;
		m_boundingbox.left = x;
		ApplyWindowSize();
	}
	void Form::setY(int y)
	{
		m_boundingbox.bottom += y - m_boundingbox.top;
		m_boundingbox.top = y;
		ApplyWindowSize();
	}
	int Form::getX()
	{
		return m_boundingbox.left;
	}
	int Form::getY()
	{
		return m_boundingbox.top;
	}
	void Form::setSize(int w, int h)
	{
		m_boundingbox.right = m_boundingbox.left + w;
		m_boundingbox.bottom = m_boundingbox.top + h;
		ApplyWindowSize();
	}
	void Form::setW(int w)
	{
		m_boundingbox.right = m_boundingbox.left + w;
		ApplyWindowSize();
	}
	void Form::setH(int h)
	{
		m_boundingbox.bottom = m_boundingbox.top + h;
		ApplyWindowSize();
	}
	int Form::getW()
	{
		return m_boundingbox.right - m_boundingbox.left;
	}
	int Form::getH()
	{
		return m_boundingbox.bottom - m_boundingbox.top;
	}
	void Form::setRect(int x, int y, int w, int h)
	{
		m_boundingbox.right = x + w;
		m_boundingbox.left = x;
		m_boundingbox.bottom = y + w;
		m_boundingbox.top = y;
		ApplyWindowSize();
	}
	void Form::setText(const WCHAR text[])
	{
		m_windowName = text;
		SetWindowText(m_hwnd, text);
	}
	const std::wstring& Form::getText()
	{
		return m_windowName;
	}
	HWND Form::getHWND()
	{
		return m_hwnd;
	}
	void Form::setParent(FormContainer::P parent)
	{
		m_parent = parent;
	}

#pragma endregion
}