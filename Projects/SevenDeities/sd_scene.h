#pragma once

#include "octdoc.h"

namespace sd
{
	class SD_Scene :public gfw::Scene
	{
		gfw::Controller m_controller;
		gfw::Camera m_camera;
		std::shared_ptr<gfw::Renderer> m_renderer;

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