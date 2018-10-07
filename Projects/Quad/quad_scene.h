#pragma once

#include "quad_quad.h"

namespace quad
{
	class Scene :public form::Scene
	{
		SHARED_ONLY(Scene);

		gfw::Controller m_controller;
		gfw::Camera m_camera;
		gfw::Renderer::P m_renderer;
		gfw::Entity::P m_plain;

		Quadruped m_quad;

	private:
		Scene() = default;

	public:
		static Scene::P Create();

		virtual void Start() override;
		virtual void Update(float deltaTime, float totalTime) override;
		virtual void HandleMessage(hcs::Input& input) override;
	};
}