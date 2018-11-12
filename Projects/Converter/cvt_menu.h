#pragma once
#include "cvt_gfxscreen.h"

namespace cvt
{
	class Menu :public form::Window
	{
		SHARED_ONLY(Menu)

		GfxScreen::P m_renderScreen;

		form::TextBox::P m_tbFilename;
		form::CheckBox::P m_cbPosition;
		form::CheckBox::P m_cbTexture;
		form::CheckBox::P m_cbNormal;
		form::CheckBox::P m_cbNormalmap;
		form::Button::P m_btnExport;

		form::ListBox::P m_lstTextureNames;

		form::Button::P m_btnExit;

	private:
		Menu(form::WindowSettings& settings);
		void Init(int width, int height);
		void ExportButtonAction();

	public:
		static Menu::P Create();

		virtual void MessageHandler(UINT msg, WPARAM wparam, LPARAM lparam) override;
		void UpdateModelInfo(LPCWSTR filename, UINT shaderInputType, ModelManager& modelManager);
	};
}