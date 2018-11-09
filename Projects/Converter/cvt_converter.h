#pragma once

#include "cvt_modelmanager.h"
#include <iostream>
#include <shellapi.h>

namespace cvt
{
	class Converter :public form::Scene
	{
		SHARED_ONLY(Converter);

		gfx::Camera m_camera;
		float m_camDistance;
		gfx::Renderer::P m_renderer;
		gfx::Entity::P m_entity;

		ModelManager m_modelManager;

	private:
		Converter() = default;
		void DropFileEvent(HDROP hDrop);
		void LoadModel(const WCHAR* filename);
		bool HandleCamera(MSG& msg);

	public:
		static Converter::P Create();

		virtual void Start() override;
		virtual void Frame(float deltaTime) override;
		virtual void MessageHandler(hcs::Input& input) override;
	};
}