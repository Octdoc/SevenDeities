#pragma once

#include "cvt_modelmanager.h"
#include <shellapi.h>

namespace cvt
{
	class GfxScreen :public form::Window
	{
		SHARED_ONLY(GfxScreen)

		gfx::Camera m_camera;
		float m_camDistance;
		gfx::Renderer::P m_renderer;
		gfx::Entity::P m_entity;

		ModelManager m_modelManager;

	private:
		GfxScreen(form::WindowSettings& settings, form::Form::P parent);
		void DropFileEvent(HDROP hDrop);
		bool HandleCamera(UINT msg, WPARAM wparam, LPARAM lparam);

	public:
		static GfxScreen::P Create(form::Form::P parent);

		virtual void MessageHandler(UINT msg, WPARAM wparam, LPARAM lparam) override;
		void LoadModel(const WCHAR* filename);
		void Render();

		void ExportModel(const WCHAR *filename, UINT shaderInputLayout);
	};
}