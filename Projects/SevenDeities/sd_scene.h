#pragma once

#include "octdoc.h"

namespace sd
{
	class SD_Scene :public gfw::Scene
	{
		SHARED_ONLY(SD_Scene);
		gfw::Controller m_controller;
		gfw::Camera m_camera;
		std::shared_ptr<gfw::Renderer> m_renderer;

	private:
		SD_Scene() = default;
		void Test();

	public:
		static SD_Scene::P Create();

		virtual void Start();
		virtual void Quit();
		virtual void Update(double deltaTime, double totalTime) override;
		virtual void Render() override;
		virtual void MessageHandler(MSG& message) override;
	};
}