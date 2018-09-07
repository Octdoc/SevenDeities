#pragma once

#include "helpers/hcs_input.h"
#include "helpers/mth_position.h"

namespace gfw
{
	class Controller
	{
		mth::Position* m_target;

		unsigned char m_key_forward;
		unsigned char m_key_back;
		unsigned char m_key_left;
		unsigned char m_key_right;
		unsigned char m_key_up;
		unsigned char m_key_down;
		unsigned char m_key_jump;
		unsigned char m_key_run;

		float m_speed;
		float m_sensitivity;

	public:
		Controller();

		mth::Position* getTarget();
		unsigned char getKey_forward();
		unsigned char getKey_back();
		unsigned char getKey_left();
		unsigned char getKey_right();
		unsigned char getKey_up();
		unsigned char getKey_down();
		unsigned char getKey_jump();
		unsigned char getKey_run();
		float getSpeed();
		float getSensitivity();
		void setTarget(mth::Position* target);
		void setKey_forward(unsigned char key);
		void setKey_back(unsigned char key);
		void setKey_left(unsigned char key);
		void setKey_right(unsigned char key);
		void setKey_up(unsigned char key);
		void setKey_down(unsigned char key);
		void setKey_jump(unsigned char key);
		void setKey_run(unsigned char key);
		void setSpeed(float speed);
		void setSensitivity(float sensitivity);

		void Update_FirstPersonMode_Fly(hcs::Input& input, float deltaTime);
		void Update_FirstPersonMode_Gravity(hcs::Input& input, float deltaTime);
		bool Update_LookAt(MSG& msg);
	};
}