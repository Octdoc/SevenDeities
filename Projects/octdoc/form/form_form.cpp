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
		void Form::ApplyWindowSize(RECT rect)
		{
			MoveWindow(m_hwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, FALSE);
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
			RECT rect;
			GetWindowRect(m_hwnd, &rect);
			rect.right += x - rect.left;
			rect.left = x;
			rect.bottom += y - rect.top;
			rect.top = y;
			ApplyWindowSize(rect);
		}
		void Form::setX(int x)
		{
			RECT rect;
			GetWindowRect(m_hwnd, &rect);
			rect.right += x - rect.left;
			rect.left = x;
			ApplyWindowSize(rect);
		}
		void Form::setY(int y)
		{
			RECT rect;
			GetWindowRect(m_hwnd, &rect);
			rect.bottom += y - rect.top;
			rect.top = y;
			ApplyWindowSize(rect);
		}
		int Form::getX()
		{
			RECT rect;
			GetWindowRect(m_hwnd, &rect);
			return rect.left;
		}
		int Form::getY()
		{
			RECT rect;
			GetWindowRect(m_hwnd, &rect);
			return rect.top;
		}
		void Form::setSize(int w, int h)
		{
			RECT rect;
			GetWindowRect(m_hwnd, &rect);
			rect.right = rect.left + w;
			rect.bottom = rect.top + h;
			ApplyWindowSize(rect);
		}
		void Form::setW(int w)
		{
			RECT rect;
			GetWindowRect(m_hwnd, &rect);
			rect.right = rect.left + w;
			ApplyWindowSize(rect);
		}
		void Form::setH(int h)
		{
			RECT rect;
			GetWindowRect(m_hwnd, &rect);
			rect.bottom = rect.top + h;
			ApplyWindowSize(rect);
		}
		int Form::getW()
		{
			RECT rect;
			GetWindowRect(m_hwnd, &rect);
			return rect.right - rect.left;
		}
		int Form::getH()
		{
			RECT rect;
			GetWindowRect(m_hwnd, &rect);
			return rect.bottom - rect.top;
		}
		void Form::setRect(int x, int y, int w, int h)
		{
			RECT rect;
			GetWindowRect(m_hwnd, &rect);
			rect.right = x + w;
			rect.left = x;
			rect.bottom = y + w;
			rect.top = y;
			ApplyWindowSize(rect);
		}
		void Form::setText(const WCHAR text[])
		{
			SetWindowText(m_hwnd, text);
		}
		const std::wstring Form::getText()
		{
			WCHAR text[256];
			GetWindowText(m_hwnd, text, 256);
			return text;
		}
		HWND Form::getHWND()
		{
			return m_hwnd;
		}

#pragma endregion
	}
}