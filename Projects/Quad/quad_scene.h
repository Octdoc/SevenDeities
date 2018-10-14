#pragma once

#include "quad_quad.h"
#include "quad_movement.h"
#include "quad_walkscript.h"

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
		bool m_running;
		WalkManager m_walk;

		form::Label::P m_labels[3];

	private:
		Scene() = default;

	public:
		static Scene::P Create();
		static Scene::P Create(form::Label::P labels[]);

		virtual void Start() override;
		virtual void Update(float deltaTime, float totalTime) override;
		virtual void HandleMessage(hcs::Input& input) override;
	};
}