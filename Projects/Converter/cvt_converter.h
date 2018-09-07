#pragma once

#include "cvt_modelmanager.h"
#include <iostream>
#include <shellapi.h>

namespace cvt
{
	class Converter :public gfw::Scene
	{
		gfw::Controller m_controller;
		gfw::Camera m_camera;
		std::shared_ptr<gfw::Renderer> m_renderer;

	private:
		void Test();
		void DropFileEvent(HDROP hDrop);

	public:
		virtual void Start() override;
		virtual void Quit() override;
		virtual void Update(double deltaTime, double totalTime) override;
		virtual void Render() override;
		virtual void MessageHandler(MSG& message) override;
	};
}