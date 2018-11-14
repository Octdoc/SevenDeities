#include "cvt_menu.h"

namespace cvt
{
	Menu::Menu(form::WindowSettings& settings)
		:form::Window(settings, nullptr) {}
	void Menu::Init(int width, int height)
	{
		auto window = std::dynamic_pointer_cast<form::Window>(m_self.lock());

		m_tbFilename = form::TextBox::Create(window, width - 190, 10, 180, 20);
		m_cbPosition = form::CheckBox::Create(window, L"Position", width - 190, 35, 100, 20);
		m_cbTexture = form::CheckBox::Create(window, L"Texture", width - 190, 55, 100, 20);
		m_cbNormal = form::CheckBox::Create(window, L"Normal", width - 190, 75, 100, 20);
		m_cbNormalmap = form::CheckBox::Create(window, L"Normalmap", width - 190, 95, 100, 20);
		m_btnExport = form::Button::Create(window, L"Export", width - 80, 35, 70, 38);
		m_btnClear = form::Button::Create(window, L"Clear", width - 80, 77, 70, 38);

		m_lstTextureNames = form::ListBox::Create(window, width - 190, 120, 180, 180);

		m_btnExit = form::Button::Create(window, L"Exit", width - 190, 310, 180, 30);

		m_renderScreen = GfxScreen::Create(window);
	}
	void Menu::ExportButtonAction()
	{
		UINT shaderInputType = 0;
		if (m_cbPosition->isChecked())
			shaderInputType |= (UINT)gfx::ShaderInputLayoutType::POSITION;
		if (m_cbTexture->isChecked())
			shaderInputType |= (UINT)gfx::ShaderInputLayoutType::TEXCOORD;
		if (m_cbNormal->isChecked())
			shaderInputType |= (UINT)gfx::ShaderInputLayoutType::NORMAL;
		if (m_cbNormalmap->isChecked())
			shaderInputType |= (UINT)gfx::ShaderInputLayoutType::NORMALMAP;
		m_renderScreen->ExportModel(m_tbFilename->getText().c_str(), shaderInputType);
	}
	void Menu::ClearButtonAction()
	{
		m_tbFilename->setText(L"");
		m_cbPosition->setCheck(false);
		m_cbTexture->setCheck(false);
		m_cbNormal->setCheck(false);
		m_cbNormalmap->setCheck(false);
		m_lstTextureNames->Clear();
		m_renderScreen->ClearModel();
	}
	Menu::P Menu::Create()
	{
		int width = 1280;
		int height = 720;
		form::WindowSettings settings;
		settings.width = width;
		settings.height = height;
		settings.windowName = L"Converter";
		Menu::P window = std::make_shared<Menu>(settings);
		window->m_self = window;
		window->Init(width, height);
		Program::Instance().AddChild(window);
		return window;
	}
	void Menu::MessageHandler(UINT msg, WPARAM wparam, LPARAM lparam)
	{
		form::Window::MessageHandler(msg, wparam, lparam);
		switch (msg)
		{
		case WM_PAINT:
			ValidateRect(m_renderScreen->getHWND(), NULL);
			m_renderScreen->Render();
			break;
		case WM_COMMAND:
			if ((HMENU)LOWORD(wparam) == m_btnExit->getID())
				PostQuitMessage(0);
			else if ((HMENU)LOWORD(wparam) == m_btnExport->getID())
				ExportButtonAction();
			else if ((HMENU)LOWORD(wparam) == m_btnClear->getID())
				ClearButtonAction();
			break;
		}
	}
	void Menu::UpdateModelInfo(LPCWSTR filename, UINT shaderInputType, ModelManager& modelManager)
	{
		m_tbFilename->setText(filename);
		m_cbPosition->setCheck(shaderInputType & (UINT)gfx::ShaderInputLayoutType::POSITION);
		m_cbTexture->setCheck(shaderInputType & (UINT)gfx::ShaderInputLayoutType::TEXCOORD);
		m_cbNormal->setCheck(shaderInputType & (UINT)gfx::ShaderInputLayoutType::NORMAL);
		m_cbNormalmap->setCheck(shaderInputType & (UINT)gfx::ShaderInputLayoutType::NORMALMAP);
		m_lstTextureNames->Clear();
		for (UINT i = 0; i < modelManager.getModelPartCount(); i++)
			m_lstTextureNames->AddString(modelManager.getTextureName(i).c_str());
	}
}
