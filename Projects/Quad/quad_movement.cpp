#include "quad_movement.h"

namespace quad
{
	void MovementWalk::Part0(float delta)
	{
		m_quad->getEntity()->MoveForward(TimeInPrevPart(delta)*0.5f);
		m_quad->getLegRF().setPosition({ 0.9f, -m_bellyy, 0.9f });
		m_quad->getLegRB().setPosition({ 0.9f, -m_bellyy, -0.9f });
		m_quad->getLegLF().setPosition({ -0.9f, -m_bellyy, 0.4f });

		float t, z;
		t = fmod(m_time, 1.0f);
		z = t - 1.4f;
		m_quad->getLegLB().setPosition({ -0.9f, Y(t), z });
	}
	void MovementWalk::Part1(float delta)
	{
		m_quad->getLegRF().setPosition({ 0.9f, -m_bellyy, 0.9f });
		m_quad->getLegRB().setPosition({ 0.9f, -m_bellyy, -0.9f });
		m_quad->getLegLB().setPosition({ -0.9f, -m_bellyy, -0.4f });

		float t, z;
		t = fmod(m_time, 1.0f);
		z = t + 0.4f;
		m_quad->getLegLF().setPosition({ -0.9f, Y(t), z });
	}
	void MovementWalk::Part2(float delta)
	{
		float t = TimeInThisPart(delta)*0.5f;
		m_quad->getEntity()->MoveForward(t);
		m_quad->getLegRF().MovePosition({ 0.0f, 0.0f, -t });
		m_quad->getLegRB().MovePosition({ 0.0f, 0.0f, -t });
		m_quad->getLegLF().MovePosition({ 0.0f, 0.0f, -t });
		m_quad->getLegLB().MovePosition({ 0.0f, 0.0f, -t });
	}
	void MovementWalk::Part3(float delta)
	{
		m_quad->getEntity()->MoveForward(TimeInPrevPart(delta)*0.5f);
		m_quad->getLegRF().setPosition({ 0.9f, -m_bellyy, 0.4f });
		m_quad->getLegLF().setPosition({ -0.9f, -m_bellyy, 0.9f });
		m_quad->getLegLB().setPosition({ -0.9f, -m_bellyy, -0.9f });

		float t, z;
		t = fmod(m_time, 1.0f);
		z = t - 1.4f;
		m_quad->getLegRB().setPosition({ 0.9f, Y(t), z });
	}
	void MovementWalk::Part4(float delta)
	{
		m_quad->getLegRB().setPosition({ 0.9f, -m_bellyy, -0.4f });
		m_quad->getLegLF().setPosition({ -0.9f, -m_bellyy, 0.9f });
		m_quad->getLegLB().setPosition({ -0.9f, -m_bellyy, -0.9f });

		float t, z;
		t = fmod(m_time, 1.0f);
		z = t + 0.4f;
		m_quad->getLegRF().setPosition({ 0.9f, Y(t), z });
	}
	void MovementWalk::Part5(float delta)
	{
		float t = TimeInThisPart(delta)*0.5f;
		m_quad->getEntity()->MoveForward(t);
		m_quad->getLegRF().MovePosition({ 0.0f, 0.0f, -t });
		m_quad->getLegRB().MovePosition({ 0.0f, 0.0f, -t });
		m_quad->getLegLF().MovePosition({ 0.0f, 0.0f, -t });
		m_quad->getLegLB().MovePosition({ 0.0f, 0.0f, -t });
	}
	float MovementWalk::TimeInThisPart(float delta)
	{
		float tmp = fmod(m_time, 1.0f);
		if (tmp - delta < 0.0f)
			return tmp;
		return delta;
	}
	float MovementWalk::TimeInPrevPart(float delta)
	{
		float tmp = fmod(m_time, 1.0f);
		if (tmp - delta < 0.0f)
			return delta - tmp;
		return 0.0f;
	}
	float MovementWalk::Y(float t)
	{
		t = 2.0f*t - 1.0f;
		return -m_bellyy + (1.0f - t * t)*m_legLift;
	}
	void MovementWalk::Init(Quadruped* quadruped)
	{
		m_length = 6.0f;
		m_speed = 3.0f;
		m_time = 0.0f;
		m_bellyy = 0.3f;
		m_legLift = 0.15f;
		m_quad = quadruped;
		m_quad->getLegRF().setPosition({ 0.9f, -m_bellyy, 0.4f });
		m_quad->getLegRB().setPosition({ 0.9f, -m_bellyy, -0.4f });
		m_quad->getLegLF().setPosition({ -0.9f, -m_bellyy, 0.9f });
		m_quad->getLegLB().setPosition({ -0.9f, -m_bellyy, -0.9f });
	}

	void MovementWalk::Update(float deltaTime)
	{
		deltaTime *= m_speed;
		m_time = fmodf(m_time + deltaTime, m_length);
		switch ((int)m_time)
		{
		case 0:
			Part0(deltaTime);
			break;
		case 1:
			Part1(deltaTime);
			break;
		case 2:
			Part2(deltaTime);
			break;
		case 3:
			Part3(deltaTime);
			break;
		case 4:
			Part4(deltaTime);
			break;
		case 5:
			Part5(deltaTime);
			break;
		}
	}

}
