#pragma once

#include "quad_quad.h"

namespace quad
{
	class MovementWalk
	{
		float m_length;
		float m_time;
		float m_speed;
		float m_bellyy;
		float m_legLift;
		Quadruped* m_quad;

	private:
		void Part0(float delta);
		void Part1(float delta);
		void Part2(float delta);
		void Part3(float delta);
		void Part4(float delta);
		void Part5(float delta);

		float TimeInThisPart(float delta);
		float TimeInPrevPart(float delta);
		float Y(float t);

	public:
		void Init(Quadruped* quadruped);
		void Update(float deltaTime);

	};
}