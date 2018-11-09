#include "gfx_controller.h"
#include <windowsx.h>

namespace octdoc
{
	namespace gfx
	{
		Controller::Controller() :m_target(NULL), m_key_forward('W'), m_key_back('S'), m_key_left('A'), m_key_right('D'),
			m_key_up(VK_SPACE), m_key_down(VK_CONTROL), m_key_jump(VK_SPACE), m_key_run(VK_SHIFT),
			m_distance(10.0f), m_speed(2.0f), m_sensitivity(0.008f) {}

		mth::Position* Controller::getTarget() { return m_target; }
		unsigned char Controller::getKey_forward() { return m_key_forward; }
		unsigned char Controller::getKey_back() { return m_key_back; }
		unsigned char Controller::getKey_left() { return 0; }
		unsigned char Controller::getKey_right() { return m_key_right; }
		unsigned char Controller::getKey_up() { return m_key_up; }
		unsigned char Controller::getKey_down() { return m_key_down; }
		unsigned char Controller::getKey_jump() { return m_key_jump; }
		unsigned char Controller::getKey_run() { return m_key_run; }
		float Controller::getSpeed() { return m_speed; }
		float Controller::getSensitivity() { return m_sensitivity; }
		void Controller::setTarget(mth::Position* target) { m_target = target; }
		void Controller::setKey_forward(unsigned char key) { m_key_forward = key; }
		void Controller::setKey_back(unsigned char key) { m_key_back = key; }
		void Controller::setKey_left(unsigned char key) { m_key_left = key; }
		void Controller::setKey_right(unsigned char key) { m_key_right = key; }
		void Controller::setKey_up(unsigned char key) { m_key_up = key; }
		void Controller::setKey_down(unsigned char key) { m_key_down = key; }
		void Controller::setKey_jump(unsigned char key) { m_key_jump = key; }
		void Controller::setKey_run(unsigned char key) { m_key_run = key; }
		void Controller::setSpeed(float speed) { m_speed = speed; }
		void Controller::setSensitivity(float sensitivity) { m_sensitivity = sensitivity; }

		void Controller::HandleMouseMove(hcs::Input& input)
		{
			if (input.isLMouseBtnDown())
			{
				m_target->LookUp(-input.getMDY() * m_sensitivity);
				m_target->TurnRight(input.getMDX()*m_sensitivity);
			}
		}

		void Controller::Update_FirstPersonMode_Fly(hcs::Input& input, float deltaTime)
		{
			float forward = 0.0f;
			float right = 0.0f;
			float upward = 0.0f;
			float delta = m_speed * deltaTime;

			if (input.isPressed(m_key_run))
				delta *= 5;
			if (input.isPressed(m_key_forward))
				forward += delta;
			if (input.isPressed(m_key_back))
				forward -= delta;
			if (input.isPressed(m_key_right))
				right += delta;
			if (input.isPressed(m_key_left))
				right -= delta;
			if (input.isPressed(m_key_down))
				upward -= delta;
			if (input.isPressed(m_key_up))
				upward += delta;
			m_target->MoveForward(forward);
			m_target->MoveRight(right);
			m_target->MoveUp(upward);
		}

		void Controller::Update_FirstPersonMode_Gravity(hcs::Input& input, float deltaTime)
		{
			float forward = 0.0f;
			float right = 0.0f;
			float upward = 0.0f;
			float delta = deltaTime * m_speed;

			if (input.isPressed(m_key_run))
				delta *= 5;
			if (input.isPressed(m_key_forward))
				forward += delta;
			if (input.isPressed(m_key_back))
				forward -= delta;
			if (input.isPressed(m_key_right))
				right += delta;
			if (input.isPressed(m_key_left))
				right -= delta;
			m_target->MoveForward(forward);
			m_target->MoveRight(right);
			m_target->MoveUp(upward);
		}
		bool Controller::Update_LookAt(hcs::Input& input)
		{
			switch (input.getMSG().message)
			{
			case WM_MOUSEWHEEL:
				if (0 < GET_WHEEL_DELTA_WPARAM(input.getMSG().wParam))
					m_distance *= 1.1f;
				else
					m_distance /= 1.1f;
				m_target->position = mth::float3x3::Rotation(m_target->rotation)*mth::float3(0.0f, 0.0f, -m_distance);
				return true;
			case WM_MOUSEMOVE:
				if (input.getMSG().wParam & MK_LBUTTON)
				{
					m_target->LookUp(-input.getMDY() * m_sensitivity);
					m_target->TurnRight(input.getMDX()*m_sensitivity);
					m_target->position = mth::float3x3::Rotation(m_target->rotation)*mth::float3(0.0f, 0.0f, -m_distance);
					return true;
				}
				return false;
			}
			return false;
		}
	}
}