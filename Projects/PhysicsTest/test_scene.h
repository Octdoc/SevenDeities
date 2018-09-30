#pragma once

#include "octdoc.h"

namespace test
{
	class Test_Scene :public gfw::Scene
	{
		SHARED_ONLY(Test_Scene);
		gfw::Controller m_controller;
		gfw::Camera m_camera;
		gfw::Renderer::P m_renderer;

		gfw::Entity::P m_gfxPlayer;
		pfw::Player::P m_phyPlayer;
		pfw::CollisionArea::P m_physicsArea;

	private:
		Test_Scene() = default;
		void Test();

	public:
		static Test_Scene::P Create();

		virtual void Start() override;
		virtual void Quit() override;
		virtual void Update(double deltaTime, double totalTime) override;
		virtual void Render() override;
		virtual void MessageHandler(MSG& message) override;
	};
}