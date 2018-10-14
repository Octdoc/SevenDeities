#pragma once

#include "octdoc.h"
#include <array>

namespace quad
{
	enum LegID
	{
		LID_RF = 0,
		LID_LF = 1,
		LID_RB = 2,
		LID_LB = 3
	};

	class Leg
	{
		gfw::Entity::P m_shouder;
		gfw::Entity::P m_thigh;
		gfw::Entity::P m_toe;

		float m_a1, m_a2, m_a3;
		float m_ox, m_oy, m_oz;
		float m_o1, m_o2, m_o3;
		float m_o3x;

		mth::float3 m_joints[2];
		bool m_valid[2];
		mth::float3 m_position;
		int m_chosenJoint;

	private:
		void SetJointRotation();
		void ForwardGeometry();

		void CalculateKneeAngle(int index, float l);
		bool InverseBaseAngle();
		bool InverseShoulderAngle();
		void InverseKneeAngle();
		void InverseGeometry();

	public:
		void InitRF(ID3D11Device *device);
		void InitLF(ID3D11Device *device);
		void InitRB(ID3D11Device *device);
		void InitLB(ID3D11Device *device);

		void Install(gfw::Entity::P body);

		void setJointStates(mth::float3 joints);
		void setJointStates(int index);
		mth::float3 getJointStates();
		mth::float3 getJointStates(int index);
		bool isValid();
		bool isValid(int index);
		void MoveJoints(mth::float3 delta);
		void setPosition(mth::float3 position);
		mth::float3 getPosition();
		void MovePosition(mth::float3 delta);
	};

	class Quadruped
	{
		gfw::Entity::P m_body;
		std::array<Leg, 4> m_legs;

	public:
		Quadruped() = default;
		void Init(ID3D11Device* device);

		gfw::Entity::P getEntity();
		Leg& getLegRF();
		Leg& getLegLF();
		Leg& getLegRB();
		Leg& getLegLB();
		Leg& getLeg(UINT index);
		std::array<Leg, 4>& getLegs();
	};
}