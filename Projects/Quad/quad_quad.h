#pragma once

#include "octdoc.h"
#include <array>

namespace quad
{
	class Leg
	{
		gfw::Entity::P m_shouder;
		gfw::Entity::P m_thigh;
		gfw::Entity::P m_toe;

		float m_a1, m_a2, m_a3;
		float m_ox, m_oy, m_oz;
		float m_o1, m_o2, m_o3;
		float m_o3x;

	public:
		void InitRF(ID3D11Device *device);
		void InitLF(ID3D11Device *device);
		void InitRB(ID3D11Device *device);
		void InitLB(ID3D11Device *device);

		void Install(gfw::Entity::P body);
	};

	class Quadruped
	{
		gfw::Entity::P m_body;
		std::array<Leg, 4> m_legs;

	public:
		Quadruped() = default;
		void Init(ID3D11Device* device);

		gfw::Entity::P getEntity();
	};
}