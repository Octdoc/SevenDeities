#include "quad_quad.h"

namespace quad
{
#pragma region Leg

	void Leg::SetJointRotation()
	{
		m_shouder->rotation.y = m_a1 + m_joints.x;
		m_thigh->rotation.x = m_a2 + m_joints.y;
		m_toe->rotation.x = m_a3 + m_joints.z;
	}

	void Leg::ForwardGeometry()
	{
		mth::float4x4 t0 = mth::float4x4::Translation(m_ox, m_oy, m_oz);
		mth::float4x4 r1 = mth::float4x4::RotationY(m_a1 + m_joints.x);
		mth::float4x4 t1 = mth::float4x4::Translation(m_o1, 0.0f, 0.0f);
		mth::float4x4 r2 = mth::float4x4::RotationZ(m_a2 + m_joints.y);
		mth::float4x4 t2 = mth::float4x4::Translation(m_o2, 0.0f, 0.0f);
		mth::float4x4 r3 = mth::float4x4::RotationZ(m_a3 + m_joints.z);
		mth::float4x4 t3 = mth::float4x4::Translation(m_o3, 0.0f, 0.0f);

		mth::float4 p = { 0.0f, 0.0f, 0.0f, 1.0f };
		mth::float4x4 t = t0 * r1*t1*r2*t2*r3*t3;
		p = t * p;
		m_position = p;
	}

	void Leg::InverseGeometry()
	{
	}

	void Leg::InitRF(ID3D11Device *device)
	{
		m_ox = 0.35f;
		m_oy = 0.21f;
		m_oz = 0.4f;
		m_o1 = 0.18f;
		m_o2 = 0.4f;
		m_o3 = 0.8f;
		m_o3x = 0.04f;
		m_a1 = mth::pi*0.25f;
		m_a2 = 0.0f;
		m_a3 = mth::pi*0.25f;

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

		setJointStates({ 0.0f, 0.0f, 0.0f });
	}
	void Leg::InitLF(ID3D11Device *device)
	{
		m_ox = -0.35f;
		m_oy = 0.21f;
		m_oz = 0.4f;
		m_o1 = 0.18f;
		m_o2 = 0.4f;
		m_o3 = 0.8f;
		m_o3x = -0.04f;
		m_a1 = mth::pi*1.75f;
		m_a2 = 0.0f;
		m_a3 = mth::pi*0.25f;

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

		setJointStates({ 0.0f, 0.0f, 0.0f });
	}
	void Leg::InitRB(ID3D11Device *device)
	{
		m_ox = 0.35f;
		m_oy = 0.21f;
		m_oz = -0.4f;
		m_o1 = 0.18f;
		m_o2 = 0.4f;
		m_o3 = 0.8f;
		m_o3x = -0.04f;
		m_a1 = mth::pi*0.75f;
		m_a2 = 0.0f;
		m_a3 = mth::pi*0.25f;

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

		setJointStates({ 0.0f, 0.0f, 0.0f });
	}
	void Leg::InitLB(ID3D11Device *device)
	{
		m_ox = -0.35f;
		m_oy = 0.21f;
		m_oz = -0.4f;
		m_o1 = 0.18f;
		m_o2 = 0.4f;
		m_o3 = 0.8f;
		m_o3x = 0.04f;
		m_a1 = mth::pi*1.25f;
		m_a2 = 0.0f;
		m_a3 = mth::pi*0.25f;

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

		setJointStates({ 0.0f, 0.0f, 0.0f });
	}
	void Leg::Install(gfw::Entity::P body)
	{
		body->AddSubpart(m_shouder);
		m_shouder->AddSubpart(m_thigh);
		m_thigh->AddSubpart(m_toe);
	}

	void Leg::setJointStates(mth::float3 joints)
	{
		m_joints = joints;
		ForwardGeometry();
		SetJointRotation();
	}

	mth::float3 Leg::getJointStates()
	{
		return m_joints;
	}

	void Leg::setPosition(mth::float3 position)
	{
		m_position = position;
		InverseGeometry();
		SetJointRotation();
	}

	mth::float3 Leg::getPosition()
	{
		return m_position;
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

	Leg& Quadruped::getLegRF()
	{
		return m_legs[0];
	}
	Leg& Quadruped::getLegLF()
	{
		return m_legs[1];
	}
	Leg& Quadruped::getLegRB()
	{
		return m_legs[2];
	}
	Leg& Quadruped::getLegLB()
	{
		return m_legs[3];
	}
	Leg& Quadruped::getLeg(UINT index)
	{
		return m_legs[index];
	}
	std::array<Leg, 4> Quadruped::getLegs()
	{
		return m_legs;
	}

#pragma endregion
}