#include "gfw_controller.h"
#include <windowsx.h>

namespace gfw
{

	Controller::Controller() :m_target(NULL), m_key_forward('W'), m_key_back('S'), m_key_left('A'), m_key_right('D'),
		m_key_up(VK_SPACE), m_key_down(VK_CONTROL), m_key_jump(VK_SPACE), m_key_run(VK_SHIFT),
		m_speed(2.0f), m_sensitivity(0.008f) {}

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
		int mx, my;
		input.getMouseDelta(mx, my);
		if (input.isLMouseBtnDown())
		{
			m_target->LookUp(-my * m_sensitivity);
			m_target->TurnRight(mx*m_sensitivity);
		}
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
		int mx, my;
		input.getMouseDelta(mx, my);
		if (input.isLMouseBtnDown())
		{
			m_target->LookUp(-my * m_sensitivity);
			m_target->TurnRight(mx*m_sensitivity);
		}
	}
	bool Controller::Update_LookAt(MSG& msg)
	{
		static int prevx = 0, prevy = 0;
		static float distance = 10.0f;

		switch (msg.message)
		{
		case WM_MOUSEWHEEL:
			if (0 < GET_WHEEL_DELTA_WPARAM(msg.wParam))
				distance *= 1.1f;
			else
				distance /= 1.1f;
			m_target->position = mth::float3x3::Rotation(m_target->rotation)*mth::float3(0.0f, 0.0f, -distance);
			return true;
		case WM_MOUSEMOVE:
			if (msg.wParam & MK_LBUTTON)
			{
				int x = GET_X_LPARAM(msg.lParam) - prevx;
				int y = GET_Y_LPARAM(msg.lParam) - prevy;
				m_target->LookUp(-y * m_sensitivity);
				m_target->TurnRight(x*m_sensitivity);
				m_target->position = mth::float3x3::Rotation(m_target->rotation)*mth::float3(0.0f, 0.0f, -distance);
				prevx += x;
				prevy += y;
				return true;
			}
			prevx = GET_X_LPARAM(msg.lParam);
			prevy = GET_Y_LPARAM(msg.lParam);
			return false;
		}
		return false;
	}
}