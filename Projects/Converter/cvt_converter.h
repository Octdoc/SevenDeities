#pragma once

#include "cvt_modelmanager.h"
#include <iostream>
#include <shellapi.h>

namespace cvt
{
	class Converter :public gfw::Scene
	{
		SHARED_ONLY(Converter);
		gfw::Camera m_camera;
		float m_camDistance;
		gfw::Renderer::P m_renderer;
		gfw::Entity::P m_entity;

		ModelManager m_modelManager;

	private:
		Converter() = default;
		void DropFileEvent(HDROP hDrop);
		void LoadModel(const WCHAR* filename);
		bool HandleCamera(MSG& msg);

	public:
		static Converter::P Create();

		virtual void Start() override;
		virtual void Quit() override;
		virtual void Update(double deltaTime, double totalTime) override;
		virtual void Render() override;
		virtual void MessageHandler(MSG& message) override;
	};
}