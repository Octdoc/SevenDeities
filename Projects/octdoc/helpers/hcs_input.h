#pragma once

#include "hcs_exception.h"

namespace octdoc
{
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

			MSG m_msg;

		private:
			void Initialize();

		public:
			Input();

			void HandleMessage();

			bool isPressed(unsigned int key);
			bool isRMouseBtnDown();
			bool isLMouseBtnDown();
			bool isMMouseBtnDown();
			void getMousePosition(int& x, int& y);
			void getMouseDelta(int& x, int& y);
			int getMX();
			int getMY();
			int getMDX();
			int getMDY();
			MSG& getMSG();
		};
	}
}