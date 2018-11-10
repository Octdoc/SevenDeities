#include "cvt_menu.h"
#include <windowsx.h>

#pragma comment(lib, "assimp.lib")

namespace cvt
{
	void GfxScreen::DropFileEvent(HDROP hDrop)
	{
		WCHAR filename[MAX_PATH];

		if (DragQueryFile(hDrop, 0xFFFFFFFF, NULL, NULL) != 1)
		{
			MessageBox(m_hwnd, L"Dropping multiple files is not supported.", L"Multiple files", MB_OK);
		}
		else
		{
			filename[0] = '\0';
			DragQueryFile(hDrop, 0, filename, MAX_PATH);
			LoadModel(filename);
		}
		DragFinish(hDrop);
	}
	void GfxScreen::LoadModel(const WCHAR* filename)
	{
		try
		{
			if (m_modelManager.LoadModelData(filename))
			{
				m_renderer->ClearEntities();
				m_entity.reset();
				m_entity = m_modelManager.CreateEntity(m_graphics);
				m_renderer->AddEntity(m_entity);
				Render();
				std::dynamic_pointer_cast<Menu>(m_parent)->UpdateModelInfo(m_modelManager.getOutFilename().c_str(), m_modelManager.getShaderInputType());
			}
		}
		catch (hcs::Exception& e)
		{
			e.ShowMessageBox();
		}
	}

	bool GfxScreen::HandleCamera(UINT msg, WPARAM wparam, LPARAM lparam)
	{
		static int prevx = 0, prevy = 0;
		float sensitivity = 0.008f;

		switch (msg)
		{
		case WM_MOUSEWHEEL:
			if (0 > GET_WHEEL_DELTA_WPARAM(wparam))
				m_camDistance *= 1.1f;
			else
				m_camDistance /= 1.1f;
			m_camera.position = mth::float3x3::Rotation(m_camera.rotation)*mth::float3(0.0f, 0.0f, -m_camDistance);
			return true;
		case WM_MOUSEMOVE:
			if (wparam & (MK_LBUTTON | MK_RBUTTON))
			{
				int x = GET_X_LPARAM(lparam) - prevx;
				int y = GET_Y_LPARAM(lparam) - prevy;

				if (wparam & MK_RBUTTON)
				{
					m_entity->position += mth::float3x3::Rotation(m_camera.rotation) * mth::float3(x*sensitivity*m_camDistance*0.1f, -y * sensitivity*m_camDistance*0.1f, 0.0f);
				}
				else if (wparam & MK_LBUTTON)
				{
					m_camera.LookUp(-y * sensitivity);
					m_camera.TurnRight(x*sensitivity);
					m_camera.position = mth::float3x3::Rotation(m_camera.rotation) * mth::float3(0.0f, 0.0f, -m_camDistance);
				}
				prevx += x;
				prevy += y;
				return true;
			}
			prevx = GET_X_LPARAM(lparam);
			prevy = GET_Y_LPARAM(lparam);
			return false;
		}
		return false;
	}

	GfxScreen::P GfxScreen::Create(form::Form::P parent)
	{
		form::WindowSettings settings;
		RECT rect;
		GetClientRect(parent->getHWND(), &rect);
		settings.windowName = L"renderScreen";
		settings.hasFrame = false;
		settings.x = rect.left;
		settings.y = rect.top;
		settings.width = rect.right - rect.left - 200;
		settings.height = rect.bottom - rect.top;
		auto renderScreen = std::make_shared<GfxScreen>(settings, parent);
		parent->AddChild(renderScreen);
		return renderScreen;
	}

	GfxScreen::GfxScreen(form::WindowSettings& settings, form::Form::P parent)
		: form::Window(settings, parent)
	{
		gfx::GraphicsSettings gfxsettings;
		gfxsettings.width = settings.width;
		gfxsettings.height = settings.height;
		gfxsettings.vsync = false;
		InitGraphics(gfxsettings);

		ID3D11Device* device = m_graphics->getDevice();
		ID3D11DeviceContext* deviceContext = m_graphics->getDeviceContext();

		float width = (float)m_graphics->getWidth();
		float height = (float)m_graphics->getHeight();
		m_camera.Init(width / height);
		m_camDistance = 10.0f;
		m_camera.position.z = -m_camDistance;

		DragAcceptFiles(m_hwnd, TRUE);

		m_renderer = gfx::SimpleRenderer::Create(m_graphics);
		m_renderer->AddEntity(m_entity = gfx::Entity::Create(gfx::Model::Create(device, L"Media/monkey.omd"), gfx::Texture::Create2D(device, L"Media/white.png"), gfx::Texture::Create2D(device, L"Media/normal.png")));
		m_graphics->setClearColor(0.5f, 0.3f, 0.2f);
	}
	void GfxScreen::MessageHandler(UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (HandleCamera(msg, wparam, lparam))
			Render();
		switch (msg)
		{
		case WM_DROPFILES:
			DropFileEvent((HDROP)wparam);
			return;
		}
	}
	void GfxScreen::Render()
	{
		m_renderer->Render(m_graphics, m_camera);
	}
	void GfxScreen::ExportModel(const WCHAR *filename, UINT shaderInputLayout)
	{
		m_modelManager.Export(filename, shaderInputLayout);
	}
}