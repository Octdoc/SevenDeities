#pragma once

#include "hcs_helpers.h"

namespace hcs
{
	class Input
	{
		bool m_keys[256];
		bool m_rMouseBtn;
		bool m_lMouseBtn;
		bool m_mMouseBtn;

		int m_mouseX, m_mouseY;
		int m_prevMouseX, m_prevMouseY;

	public:
		void Initialize();
		void HandleMessage(MSG& msg);

		bool isPressed(unsigned int key);
		bool isRMouseBtnDown();
		bool isLMouseBtnDown();
		bool isMMouseBtnDown();
		void getMousePosition(int& x, int& y);
		void getMouseDelta(int& x, int& y);
		void ResetMouseDelta();
	};
}