#include "quad_walkscript.h"
#include <iostream>

namespace quad
{
#pragma region WalkScript

	WalkScript::WalkScript() :
		m_maxTurnAtOnce(mth::pi*0.25f),
		m_bellyy(0.3f),
		m_legLift(0.2f),
		m_legXPos(0.9f),
		m_legZRetracted(0.4f),
		m_legStretchHalf(0.5f),
		m_rightBalanced(true) {}

	void WalkScript::AddPathElementTurn(float angle)
	{
		float angleabs = fabsf(angle);
		while (angleabs > 0.0f)
		{
			float ratio = min(fabsf(angle) / m_maxTurnAtOnce, 1.0f);
			if (angle < 0.0f)
				ratio = -ratio;
			QuadAction qa;
			mth::float2x2 rotmat = mth::float2x2::Rotation(-ratio * m_maxTurnAtOnce);
			if (m_rightBalanced)
			{
				qa.legID = LID_LB;
				qa.goalPos = rotmat * mth::float2(-m_legXPos, -(m_legZRetracted + m_legStretchHalf));
				m_script.push_back(qa);
				qa.legID = LID_LF;
				qa.goalPos = rotmat * mth::float2(-m_legXPos, m_legZRetracted + m_legStretchHalf);
				m_script.push_back(qa);
				qa.legID = LID_RF;
				qa.goalPos = rotmat * mth::float2(m_legXPos, m_legZRetracted + m_legStretchHalf);
				m_script.push_back(qa);
				qa.legID = LID_RB;
				qa.goalPos = rotmat * mth::float2(m_legXPos, -(m_legZRetracted + m_legStretchHalf));
				m_script.push_back(qa);
				m_rightBalanced = angle > 0.0f;
			}
			else
			{
				qa.legID = LID_RB;
				qa.goalPos = rotmat * mth::float2(m_legXPos, -(m_legZRetracted + m_legStretchHalf));
				m_script.push_back(qa);
				qa.legID = LID_RF;
				qa.goalPos = rotmat * mth::float2(m_legXPos, m_legZRetracted + m_legStretchHalf);
				m_script.push_back(qa);
				qa.legID = LID_LF;
				qa.goalPos = rotmat * mth::float2(-m_legXPos, m_legZRetracted + m_legStretchHalf);
				m_script.push_back(qa);
				qa.legID = LID_LB;
				qa.goalPos = rotmat * mth::float2(-m_legXPos, -(m_legZRetracted + m_legStretchHalf));
				m_script.push_back(qa);
				m_rightBalanced = angle > 0.0f;
			}
			qa.legID = -1;
			qa.goalPos = 0.0f;
			qa.rot = m_maxTurnAtOnce * ratio;
			m_script.push_back(qa);
			angleabs -= m_maxTurnAtOnce;
			if (angle > 0.0f)
				angle -= m_maxTurnAtOnce;
			else
				angle += m_maxTurnAtOnce;
		}
	}

	void WalkScript::AddPathElementWalkStraight(float distance)
	{
		while (distance > 0.0f)
		{
			float ratio = min(distance / m_legStretchHalf, 1.0f);
			QuadAction qa;
			if (m_rightBalanced)
			{
				qa.legID = LID_LB;
				qa.goalPos.x = -m_legXPos;
				qa.goalPos.y = -m_legZRetracted - (1.0f - ratio)*m_legStretchHalf;
				m_script.push_back(qa);
				qa.legID = LID_LF;
				qa.goalPos.x = -m_legXPos;
				qa.goalPos.y = m_legZRetracted + 2.0f*m_legStretchHalf - (1.0f - ratio)*m_legStretchHalf;
				m_script.push_back(qa);
				m_rightBalanced = false;
			}
			else
			{
				qa.legID = LID_RB;
				qa.goalPos.x = m_legXPos;
				qa.goalPos.y = -m_legZRetracted - (1.0f - ratio)*m_legStretchHalf;
				m_script.push_back(qa);
				qa.legID = LID_RF;
				qa.goalPos.x = m_legXPos;
				qa.goalPos.y = m_legZRetracted + 2.0f*m_legStretchHalf - (1.0f - ratio)*m_legStretchHalf;
				m_script.push_back(qa);
				m_rightBalanced = true;
			}
			qa.legID = -1;
			qa.goalPos = mth::float2(0.0f, m_legStretchHalf*ratio);
			qa.rot = 0.0f;
			m_script.push_back(qa);
			distance -= m_legStretchHalf;
		}
	}

	/*void WalkScript::AddPathElement(mth::float2 relativePos, float relativeRot)
	{
		float distance = relativePos.Length();
		int count = (int)ceilf(distance / m_legStretchHalf);
		for (int i = 0; i < count - 1; i++)
		{
			QuadAction qa;
			if (m_lastLegMoved == LID_LB || m_lastLegMoved == LID_LF)
			{
				qa.legID = LID_RB;
				qa.goalPos.x = m_legXPos;
				qa.goalPos.y = -m_legZRetracted;
				m_script.push_back(qa);
				qa.legID = LID_RF;
				qa.goalPos.x = m_legXPos;
				qa.goalPos.y = m_legZRetracted + 2.0f*m_legStretchHalf;
				m_script.push_back(qa);
				m_lastLegMoved = LID_RF;
			}
			else
			{
				qa.legID = LID_LB;
				qa.goalPos.x = -m_legXPos;
				qa.goalPos.y = -m_legZRetracted;
				m_script.push_back(qa);
				qa.legID = LID_LF;
				qa.goalPos.x = -m_legXPos;
				qa.goalPos.y = m_legZRetracted + 2.0f*m_legStretchHalf;
				m_script.push_back(qa);
				m_lastLegMoved = LID_LF;
			}
			qa.legID = -1;
			qa.goalPos = relativePos / distance * m_legStretchHalf;
			qa.rot = 0.0f;
			m_script.push_back(qa);
		}

	}*/
	bool WalkScript::NextAction(QuadAction& action)
	{
		if (m_script.empty())
			return false;
		action = *m_script.begin();
		m_script.pop_front();
		return true;
	}
	mth::float3 WalkScript::getLegRFStartPos()
	{
		return { m_legXPos, -m_bellyy, m_legZRetracted + m_legStretchHalf };
	}
	mth::float3 WalkScript::getLegLFStartPos()
	{
		return { -m_legXPos, -m_bellyy, m_legZRetracted };
	}
	mth::float3 WalkScript::getLegRBStartPos()
	{
		return { m_legXPos, -m_bellyy, -(m_legZRetracted + m_legStretchHalf) };
	}
	mth::float3 WalkScript::getLegLBStartPos()
	{
		return mth::float3({ -m_legXPos, -m_bellyy, -(m_legZRetracted + 2.0f*m_legStretchHalf) });
	}
	float WalkScript::getBellyY()
	{
		return m_bellyy;
	}
	float WalkScript::LegY(float t)
	{
		t = 2.0f*t - 1.0f;
		return -m_bellyy + (1.0f - t * t)*m_legLift;
	}

#pragma endregion

#pragma region WalkManager

	void WalkManager::Init(Quadruped *quadruped)
	{
		m_quad = quadruped;
		m_time = 0.0f;
		m_speed = 3.5f;
		m_running = false;
		m_quad->getEntity()->position = { 0.0f, m_script.getBellyY(), 0.0f };
		m_quad->getLegRF().setPosition(m_script.getLegRFStartPos());
		m_quad->getLegRB().setPosition(m_script.getLegRBStartPos());
		m_quad->getLegLF().setPosition(m_script.getLegLFStartPos());
		m_quad->getLegLB().setPosition(m_script.getLegLBStartPos());
	}

	void WalkManager::MoveBody(float deltaTime)
	{
		mth::float3 delta = { -deltaTime * m_action.goalPos.x, 0.0f, deltaTime * m_action.goalPos.y };
		m_quad->getEntity()->MoveInLookDirection(delta);
		m_quad->getEntity()->rotation.y += deltaTime * m_action.rot;
		delta = mth::float3x3::RotationY(m_action.rot)*delta;
		for (Leg& l : m_quad->getLegs())
			l.setPosition(mth::float3x3::RotationY(-deltaTime * m_action.rot)*(-delta + l.getPosition()));
	}
	void WalkManager::MoveLeg()
	{
		mth::float3 pos = { m_prevPos.x + (m_action.goalPos.x - m_prevPos.x)*m_time,
			0.0f, m_prevPos.y + (m_action.goalPos.y - m_prevPos.y)*m_time };
		pos.Normalize();
		pos *= (1.0f - m_time)*m_prevPos.Length() + m_time * m_action.goalPos.Length();
		pos.y = m_script.LegY(m_time);
		m_quad->getLeg(m_action.legID).setPosition(pos);
	}
	void WalkManager::Update(float deltaTime)
	{
		deltaTime *= m_speed;
		if (!m_running)
		{
			m_script.AddPathElementWalkStraight(2.0f);
			m_script.AddPathElementTurn(-mth::pi*0.5f);
			if (m_running = m_script.NextAction(m_action))
			{
				if (m_action.legID >= 0)
				{
					mth::float3 prevPos = m_quad->getLeg(m_action.legID).getPosition();
					m_prevPos.x = prevPos.x;
					m_prevPos.y = prevPos.z;
				}
				m_time = 0.0f;
			}
		}
		if (m_running)
		{
			m_time += deltaTime;
			if (m_time >= 1.0f)
			{
				if (m_action.legID < 0)
					MoveBody(1.0f - (m_time - deltaTime));
				else
					m_quad->getLeg(m_action.legID).setPosition({ m_action.goalPos.x, -m_script.getBellyY(), m_action.goalPos.y });
				m_time = fmodf(m_time, 1.0f);
				deltaTime = m_time;
				if (m_running = m_script.NextAction(m_action))
					if (m_action.legID >= 0)
					{
						mth::float3 prevPos = m_quad->getLeg(m_action.legID).getPosition();
						m_prevPos.x = prevPos.x;
						m_prevPos.y = prevPos.z;
					}
			}
			if (m_action.legID < 0)
				MoveBody(deltaTime);
			else
				MoveLeg();
		}
	}
	WalkScript& WalkManager::getWalkScript()
	{
		return m_script;
	}

#pragma endregion
}

