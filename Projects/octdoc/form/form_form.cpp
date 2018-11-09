#include "form_form.h"

namespace octdoc
{
	namespace form
	{
		size_t Form::m_autoIDdistributor = 1;

		Form::P Form::ToForm()
		{
			return m_self.lock();
		}
		void Form::AddChild(Form::P child)
		{
			m_children.push_front(child);
		}
		void Form::RemoveChild(Form::P child)
		{
			m_children.remove(child);
		}
		void Form::RemoveAllChild()
		{
			m_children.clear();
		}
		void Form::ApplyWindowSize()
		{
			MoveWindow(m_hwnd, getX(), getY(), getW(), getH(), FALSE);
		}

		Form::Form(FormContainer::P parent)
			:m_parent(parent) {}
		Form::~Form()
		{
			DestroyWindow(m_hwnd);
			m_hwnd = nullptr;
			m_parent->RemoveChild(m_self.lock());
		}
		void Form::Close()
		{
			m_parent->RemoveChild(m_self.lock());
		}
		bool Form::MessageHandlerCaller(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			if (m_hwnd == hwnd)
			{
				MessageHandler(msg, wparam, lparam);
				return true;
			}
			for (auto c : m_children)
				if (c->MessageHandlerCaller(hwnd, msg, wparam, lparam))
					return true;
			return false;
		}
		void Form::FrameCaller(float deltaTime)
		{
			Frame(deltaTime);
			for (auto c : m_children)
				c->FrameCaller(deltaTime);
		}
		void Form::MessageHandler(UINT msg, WPARAM wparam, LPARAM lparam) {}
		void Form::Frame(float deltaTime) {}

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

#pragma endregion
	}
}