#pragma once

#include "octdoc.h"

using namespace octdoc;

namespace sd
{
	class Scene :public form::Scene
	{
		SHARED_ONLY(Scene)

		gfx::Controller m_controller;
		gfx::Camera m_camera;
		gfx::Renderer::P m_renderer;

	private:
		Scene() = default;

	public:
		static Scene::P Create();

		virtual void Start() override;
		virtual void Frame(float deltaTime) override;
		virtual void MessageHandler(hcs::Input& input) override;
	};
}