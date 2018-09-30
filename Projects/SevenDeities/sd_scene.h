#pragma once

#include "octdoc.h"

namespace sd
{
	class SD_Scene :public form::Scene
	{
		SHARED_ONLY(SD_Scene);

		gfw::Controller m_controller;
		gfw::Camera m_camera;
		gfw::Renderer::P m_renderer;

	private:
		SD_Scene() = default;

	public:
		static SD_Scene::P Create();

		virtual void Start() override;
		virtual void Update(float deltaTime, float totalTime) override;
		virtual void HandleMessage(hcs::Input& input) override;
	};
}