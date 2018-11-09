#pragma once

#include "octdoc.h"

namespace car
{
	class Scene :public form::Scene
	{
		SHARED_ONLY(Scene);

		gfw::Controller m_controller;
		gfw::Camera m_camera;
		gfw::Renderer::P m_renderer;

	private:
		Scene() = default;

	public:
		static Scene::P Create();

		virtual void Start() override;
		virtual void Frame(float deltaTime) override;
		virtual void MessageHandler(hcs::Input& input) override;
	};
}