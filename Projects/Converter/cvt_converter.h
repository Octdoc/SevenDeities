#pragma once

#include "cvt_modelmanager.h"
#include <iostream>
#include <shellapi.h>

namespace cvt
{
	class Converter :public gfw::Scene
	{
		gfw::Camera m_camera;
		float m_camDistance;
		std::shared_ptr<gfw::Renderer> m_renderer;
		std::shared_ptr<gfw::Entity> m_entity;

		ModelManager m_modelManager;

	private:
		void DropFileEvent(HDROP hDrop);
		void LoadModel(const WCHAR* filename);
		bool HandleCamera(MSG& msg);

	public:
		virtual void Start() override;
		virtual void Quit() override;
		virtual void Update(double deltaTime, double totalTime) override;
		virtual void Render() override;
		virtual void MessageHandler(MSG& message) override;
	};
}