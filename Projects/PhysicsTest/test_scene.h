#pragma once

#include "octdoc.h"

namespace test
{
	class Test_Scene :public gfw::Scene
	{
		gfw::Controller m_controller;
		gfw::Camera m_camera;
		std::shared_ptr<gfw::Renderer> m_renderer;

		std::shared_ptr<gfw::Entity> m_gfxPlayer;
		std::shared_ptr<pfw::Player> m_phyPlayer;
		pfw::CollisionArea m_physicsArea;

	private:
		void Test();

	public:
		virtual void Start() override;
		virtual void Quit() override;
		virtual void Update(double deltaTime, double totalTime) override;
		virtual void Render() override;
		virtual void MessageHandler(MSG& message) override;
	};
}