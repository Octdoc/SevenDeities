#include "hcs_input.h"
#include <windowsx.h>

namespace hcs
{
	void Input::Initialize()
	{
		POINT cursorpos;
		memset(m_keys, false, sizeof(m_keys));
		memset(&m_msg, 0, sizeof(m_msg));

		m_rMouseBtn = false;
		m_lMouseBtn = false;
		m_mMouseBtn = false;

		GetCursorPos(&cursorpos);
		m_mouseX = m_prevMouseX = cursorpos.x;
		m_mouseY = m_prevMouseY = cursorpos.y;
	}
	Input::Input()
	{
		Initialize();
	}

	void Input::HandleMessage()
	{
		switch (m_msg.message)
		{
		case WM_KEYDOWN:
			m_keys[m_msg.wParam & 0xff] = true;
			break;
		case WM_KEYUP:
			m_keys[m_msg.wParam & 0xff] = false;
			break;
		case WM_MOUSEMOVE:
			m_prevMouseX = m_mouseX;
			m_prevMouseY = m_mouseY;
			m_mouseX = GET_X_LPARAM(m_msg.lParam);
			m_mouseY = GET_Y_LPARAM(m_msg.lParam);
			break;
		case WM_RBUTTONDOWN:
			m_rMouseBtn = true;
			break;
		case WM_MBUTTONDOWN:
			m_mMouseBtn = true;
			break;
		case WM_LBUTTONDOWN:
			m_lMouseBtn = true;
			break;
		case WM_RBUTTONUP:
			m_rMouseBtn = false;
			break;
		case WM_MBUTTONUP:
			m_mMouseBtn = false;
			break;
		case WM_LBUTTONUP:
			m_lMouseBtn = false;
			break;
		}
	}

	bool Input::isPressed(unsigned int key)
	{
		return m_keys[key & 0xff];
	}
	bool Input::isRMouseBtnDown()
	{
		return m_rMouseBtn;
	}
	bool Input::isLMouseBtnDown()
	{
		return m_lMouseBtn;
	}
	bool Input::isMMouseBtnDown()
	{
		return m_mMouseBtn;
	}
	void Input::getMousePosition(int& x, int& y)
	{
		x = m_mouseX;
		y = m_mouseY;
	}
	void Input::getMouseDelta(int& x, int& y)
	{
		x = m_mouseX - m_prevMouseX;
		y = m_mouseY - m_prevMouseY;
	}
	int Input::getMDX()
	{
		return m_mouseX - m_prevMouseX;;
	}
	int Input::getMDY()
	{
		return m_mouseY - m_prevMouseY;
	}
	MSG& Input::getMSG()
	{
		return m_msg;
	}
	int Input::getMX()
	{
		return m_mouseX;
	}
	int Input::getMY()
	{
		return m_mouseY;
	}
}