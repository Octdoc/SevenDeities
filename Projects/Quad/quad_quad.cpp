#include "quad_quad.h"

namespace quad
{
#pragma region Leg

	void Leg::SetJointRotation()
	{
		m_shouder->rotation.y = m_a1 + m_joints[m_chosenJoint].x;
		m_thigh->rotation.x = m_a2 + m_joints[m_chosenJoint].y;
		m_toe->rotation.x = m_a3 + m_joints[m_chosenJoint].z;
	}

	void Leg::ForwardGeometry()
	{
		float cos1 = cosf(m_a1 + m_joints[0].x);
		float cos2 = cosf(m_a2 + m_joints[0].y);
		float cos23 = cosf(m_a2 + m_joints[0].y + m_a3 + m_joints[0].z);
		float sin1 = sinf(m_a1 + m_joints[0].x);
		float sin2 = sinf(m_a2 + m_joints[0].y);
		float sin23 = sinf(m_a2 + m_joints[0].y + m_a3 + m_joints[0].z);

		m_position.x = cos1 * m_o3x + sin1 * cos23*m_o3 + sin1 * cos2*m_o2 + sin1 * m_o1 + m_ox;
		m_position.y = -sin23 * m_o3 - sin2 * m_o2 + m_oy;
		m_position.z = -sin1 * m_o3x + cos1 * cos23*m_o3 + cos1 * cos2*m_o2 + cos1 * m_o1 + m_oz;

		m_chosenJoint = 0;
		m_valid[0] = true;
		m_valid[1] = false;
	}

	void Leg::InverseGeometry()
	{
		float a, b, d, discriminant, drt, sol;
		m_valid[0] = false;
		m_valid[1] = false;

		//first
		a = m_oz - m_position.z;
		b = m_position.x - m_ox;
		d = m_o3x;
		discriminant = a * a + b * b - d * d;
		if (discriminant < 0.0f)
			return;
		drt = sqrtf(discriminant);
		sol = atan2f((a*d + b * drt), (b * d - a * drt));
		if (sol < m_a1 - mth::pi*0.5f || sol > m_a1 + mth::pi*0.5f)
		{
			sol = atan2f((a*d - b * drt), (b * d + a * drt));
			if (sol < m_a1 - mth::pi*0.5f || sol > m_a1 + mth::pi*0.5f)
				return;
		}
		m_joints[0].x = sol;
		m_joints[1].x = sol;

		//second
		a = m_oy - m_position.y;
		b = (m_position.x - m_ox - cosf(sol)*m_o3x) / sinf(sol) - m_o1;
		d = (b*b + a * a + m_o2 * m_o2 - m_o3 * m_o3) / (2.0f*m_o2);
		discriminant = a * a + b * b - d * d;
		if (discriminant < 0.0f)
			return;
		drt = sqrtf(discriminant);
		m_joints[0].y = atan2f((a*d + b * drt), (b * d - a * drt));
		m_joints[1].y = atan2f((a*d - b * drt), (b * d + a * drt));
		m_valid[0] = (m_joints[0].y >= m_a2 - mth::pi*0.5f &&  m_joints[0].y <= m_a2 + mth::pi*0.5f);
		m_valid[1] = (m_joints[1].y >= m_a2 - mth::pi*0.5f &&  m_joints[1].y <= m_a2 + mth::pi*0.5f);

		if (!m_valid[0] && !m_valid[1])
			return;

		//third
		if (m_valid[0])
		{
			m_joints[0].z = atan2f(m_oy - m_position.y - sinf(m_joints[0].y)*m_o2, b - cosf(m_joints[0].y)*m_o2) - m_joints[0].y;
			m_joints[0].x -= m_a1;
			m_joints[0].y -= m_a2;
			m_joints[0].z -= m_a3;
			m_valid[0] = (m_joints[0].z >= m_a3 - mth::pi*0.5f) && (m_joints[0].z <= m_a3 + mth::pi*0.5f);
		}
		if (m_valid[1])
		{
			m_joints[1].z = atan2f(m_oy - m_position.y - sinf(m_joints[1].y)*m_o2, b - cosf(m_joints[1].y)*m_o2) - m_joints[1].y;
			m_joints[1].x -= m_a1;
			m_joints[1].y -= m_a2;
			m_joints[1].z -= m_a3;
			m_valid[1] = (m_joints[1].z >= m_a3 - mth::pi*0.5f) && (m_joints[1].z <= m_a3 + mth::pi*0.5f);
		}

		if (m_valid[0])
			m_chosenJoint = 0;
		else if (m_valid[1])
			m_chosenJoint = 1;
		else
			m_chosenJoint = -1;
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

		setJointStates(0.0f);
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
		m_a1 = -mth::pi*0.25f;
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

		setJointStates(0.0f);
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

		setJointStates(0.0f);
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
		m_a1 = -mth::pi*0.75f;
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

		setJointStates(0.0f);
	}
	void Leg::Install(gfw::Entity::P body)
	{
		body->AddSubpart(m_shouder);
		m_shouder->AddSubpart(m_thigh);
		m_thigh->AddSubpart(m_toe);
	}

	void Leg::setJointStates(mth::float3 joints)
	{
		m_joints[0] = joints;
		ForwardGeometry();
		SetJointRotation();
	}

	void Leg::setJointStates(int index)
	{
		m_chosenJoint = index;
		SetJointRotation();
	}

	mth::float3 Leg::getJointStates()
	{
		return m_joints;
	}

	mth::float3 Leg::getJointStates(int index)
	{
		return m_joints[index];
	}

	bool Leg::isValid()
	{
		return m_chosenJoint > 0;
	}

	bool Leg::isValid(int index)
	{
		return m_valid[index];
	}

	void Leg::MoveJoints(mth::float3 delta)
	{
		m_joints[0] = m_joints[m_chosenJoint] + delta;
		ForwardGeometry();
		SetJointRotation();
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

	void Leg::MovePosition(mth::float3 delta)
	{
		m_position += delta;
		InverseGeometry();
		SetJointRotation();
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
	std::array<Leg, 4>& Quadruped::getLegs()
	{
		return m_legs;
	}

#pragma endregion
}