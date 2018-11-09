#pragma once

#include "octdoc.h"

namespace test
{
	class Test_Scene :public form::Scene
	{
		SHARED_ONLY(Test_Scene);
		gfw::Controller m_controller;
		gfw::Camera m_camera;
		gfw::Renderer::P m_renderer;
		gfw::Entity::P m_rod;

		pfw::Collider::P m_phyFloor;
		pfw::Collider::P m_phyMonkey;
		gfw::Entity::P m_gfxPlayer;
		pfw::Player::P m_phyPlayer;
		pfw::CollisionArea::P m_physicsArea;

	private:
		Test_Scene() = default;

	public:
		static Test_Scene::P Create();

		virtual void Start() override;
		virtual void Frame(float deltaTime) override;
		virtual void MessageHandler(hcs::Input& input) override;
	};
}