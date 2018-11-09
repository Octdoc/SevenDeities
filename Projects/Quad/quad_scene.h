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
		pfw::Collider::P m_enviroment;

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
		virtual void Frame(float deltaTime) override;
		virtual void MessageHandler(hcs::Input& input) override;
	};
}