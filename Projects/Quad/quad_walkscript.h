#pragma once

#include "quad_quad.h"
#include <list>

namespace quad
{
	struct QuadAction
	{
		int legID;
		mth::float2 goalPos;
		float rot;
	};
	
	class WalkScript
	{
		float m_maxTurnAtOnce;
		float m_bellyy;
		float m_legLift;
		float m_legXPos;
		float m_legZRetracted;
		float m_legStretchHalf;
		LegID m_lastLegMoved;
		std::list<QuadAction> m_script;

	public:
		WalkScript();
		void AddPathElementTurn(float angle);
		void AddPathElementWalkStraight(float distance);
		void AddPathElement(mth::float2 relativePos, float relativeRot);
		bool NextAction(QuadAction& action);
		mth::float3 getLegRFStartPos();
		mth::float3 getLegLFStartPos();
		mth::float3 getLegRBStartPos();
		mth::float3 getLegLBStartPos();
		float getBellyY();
		float LegY(float t);
	};

	class WalkManager
	{
		float m_time;
		float m_speed;
		WalkScript m_script;
		Quadruped *m_quad;
		QuadAction m_action;
		mth::float2 m_prevPos;
		bool m_running;

	private:
		void MoveBody(float deltaTime);
		void MoveLeg(float deltaTime);

	public:
		void Init(Quadruped *quadruped);
		void Update(float deltaTime);
		WalkScript& getWalkScript();
	};
}