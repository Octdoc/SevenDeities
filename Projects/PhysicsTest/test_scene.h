#pragma once

#include "octdoc.h"

using namespace octdoc;

namespace test
{
	class Test_Scene :public form::Scene
	{
		SHARED_ONLY(Test_Scene);
		gfx::Controller m_controller;
		gfx::Camera m_camera;
		gfx::Renderer::P m_renderer;
		gfx::Entity::P m_rod;

		physx::Collider::P m_phyFloor;
		physx::Collider::P m_phyMonkey;
		gfx::Entity::P m_gfxPlayer;
		physx::Player::P m_phyPlayer;
		physx::CollisionArea::P m_physicsArea;

	private:
		Test_Scene() = default;

	public:
		static Test_Scene::P Create();

		virtual void Start() override;
		virtual void Frame(float deltaTime) override;
		virtual void MessageHandler(hcs::Input& input) override;
	};
}