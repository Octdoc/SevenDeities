#include "quad_quad.h"

namespace quad
{
#pragma region Leg

	void Leg::InitRF(ID3D11Device *device)
	{
		m_ox = 0.35f;
		m_oy = 0.21;
		m_oz = 0.4;
		m_o1 = 0.18;
		m_o2 = 0.4;
		m_o3 = 0.8;
		m_o3x = 0.04;
		m_a1 = mth::pi*0.25f;
		m_a2 = -mth::pi*0.25f;
		m_a3 = mth::pi*0.5f;

		m_shouder = gfw::Entity::Create(gfw::Model::Create(device, L"Media/quad/shoulderRF.omd"));
		m_shouder->position = { m_ox, m_oy, m_oz };
		m_shouder->setColor(1.0f);
		m_shouder->rotation.y = m_a1;

		m_thigh = gfw::Entity::Create(gfw::Model::Create(device, L"Media/quad/thighRF.omd"));
		m_thigh->position = { 0.0f, 0.0f, m_o1 };
		m_thigh->setColor(1.0f);
		m_thigh->rotation.x = m_a2;

		m_toe = gfw::Entity::Create(gfw::Model::Create(device, L"Media/quad/toeRF.omd"));
		m_toe->position = { m_o3x, 0.0f, m_o2 };
		m_toe->setColor(1.0f);
		m_toe->rotation.x = m_a3;
	}
	void Leg::InitLF(ID3D11Device *device)
	{
		m_ox = -0.35f;
		m_oy = 0.21;
		m_oz = 0.4;
		m_o1 = 0.18;
		m_o2 = 0.4;
		m_o3 = 0.8;
		m_o3x = -0.04;
		m_a1 = mth::pi*1.75f;
		m_a2 = -mth::pi*0.25f;
		m_a3 = mth::pi*0.5f;

		m_shouder = gfw::Entity::Create(gfw::Model::Create(device, L"Media/quad/shoulderLF.omd"));
		m_shouder->position = { m_ox, m_oy, m_oz };
		m_shouder->setColor(1.0f);
		m_shouder->rotation.y = m_a1;

		m_thigh = gfw::Entity::Create(gfw::Model::Create(device, L"Media/quad/thighLF.omd"));
		m_thigh->position = { 0.0f, 0.0f, m_o1 };
		m_thigh->setColor(1.0f);
		m_thigh->rotation.x = m_a2;

		m_toe = gfw::Entity::Create(gfw::Model::Create(device, L"Media/quad/toeLF.omd"));
		m_toe->position = { m_o3x, 0.0f, m_o2 };
		m_toe->setColor(1.0f);
		m_toe->rotation.x = m_a3;
	}
	void Leg::InitRB(ID3D11Device *device)
	{
		m_ox = 0.35f;
		m_oy = 0.21;
		m_oz = -0.4;
		m_o1 = 0.18;
		m_o2 = 0.4;
		m_o3 = 0.8;
		m_o3x = -0.04;
		m_a1 = mth::pi*0.75f;
		m_a2 = -mth::pi*0.25f;
		m_a3 = mth::pi*0.5f;

		m_shouder = gfw::Entity::Create(gfw::Model::Create(device, L"Media/quad/shoulderLF.omd"));
		m_shouder->position = { m_ox, m_oy, m_oz };
		m_shouder->setColor(1.0f);
		m_shouder->rotation.y = m_a1;

		m_thigh = gfw::Entity::Create(gfw::Model::Create(device, L"Media/quad/thighLF.omd"));
		m_thigh->position = { 0.0f, 0.0f, m_o1 };
		m_thigh->setColor(1.0f);
		m_thigh->rotation.x = m_a2;

		m_toe = gfw::Entity::Create(gfw::Model::Create(device, L"Media/quad/toeLF.omd"));
		m_toe->position = { m_o3x, 0.0f, m_o2 };
		m_toe->setColor(1.0f);
		m_toe->rotation.x = m_a3;
	}
	void Leg::InitLB(ID3D11Device *device)
	{
		m_ox = -0.35f;
		m_oy = 0.21;
		m_oz = -0.4;
		m_o1 = 0.18;
		m_o2 = 0.4;
		m_o3 = 0.8;
		m_o3x = 0.04;
		m_a1 = mth::pi*1.25f;
		m_a2 = -mth::pi*0.25f;
		m_a3 = mth::pi*0.5f;

		m_shouder = gfw::Entity::Create(gfw::Model::Create(device, L"Media/quad/shoulderRF.omd"));
		m_shouder->position = { m_ox, m_oy, m_oz };
		m_shouder->setColor(1.0f);
		m_shouder->rotation.y = m_a1;

		m_thigh = gfw::Entity::Create(gfw::Model::Create(device, L"Media/quad/thighRF.omd"));
		m_thigh->position = { 0.0f, 0.0f, m_o1 };
		m_thigh->setColor(1.0f);
		m_thigh->rotation.x = m_a2;

		m_toe = gfw::Entity::Create(gfw::Model::Create(device, L"Media/quad/toeRF.omd"));
		m_toe->position = { m_o3x, 0.0f, m_o2 };
		m_toe->setColor(1.0f);
		m_toe->rotation.x = m_a3;
	}
	void Leg::Install(gfw::Entity::P body)
	{
		body->AddSubpart(m_shouder);
		m_shouder->AddSubpart(m_thigh);
		m_thigh->AddSubpart(m_toe);
	}

#pragma endregion

#pragma region Quad

	void Quadruped::Init(ID3D11Device* device)
	{
		m_body = gfw::Entity::Create(gfw::Model::Create(device, L"Media/quad/body.omd"));
		m_body->setColor(1.0f);
		m_body->position.y = 0.5f;
		m_legs[0].InitRF(device);
		m_legs[0].Install(m_body);
		m_legs[1].InitLF(device);
		m_legs[1].Install(m_body);
		m_legs[2].InitRB(device);
		m_legs[2].Install(m_body);
		m_legs[3].InitLB(device);
		m_legs[3].Install(m_body);
	}
	gfw::Entity::P Quadruped::getEntity()
	{
		return m_body;
	}

#pragma endregion
}