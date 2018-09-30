#include "cvt_converter.h"
#include <windowsx.h>

#pragma comment(lib, "engine.lib")
#pragma comment(lib, "assimp.lib")

namespace cvt
{
	void Converter::DropFileEvent(HDROP hDrop)
	{
		WCHAR filename[MAX_PATH];

		if (DragQueryFile(hDrop, 0xFFFFFFFF, NULL, NULL) != 1)
		{
			std::wcout << L"Dropping multiple files is not supported." << std::endl;
		}
		else
		{
			filename[0] = '\0';
			DragQueryFile(hDrop, 0, filename, MAX_PATH);
			std::wcout << L"Loading: " << filename << std::endl;
			LoadModel(filename);
		}
		DragFinish(hDrop);
	}
	void Converter::LoadModel(const WCHAR* filename)
	{
		if (m_modelManager.LoadModelData(filename))
		{
			try
			{
				m_renderer->ClearEntities();
				m_entity.reset();
				m_entity = m_modelManager.CreateEntity(m_graphics);
				m_renderer->AddEntity(m_entity);
				m_renderer->Render(m_graphics, m_camera);
				InvalidateRect(m_window->getHWND(), NULL, TRUE);
			}
			catch (hcs::Exception& e)
			{
				std::wcout << e.Caption() << std::endl << e.Description() << std::endl;
			}
		}
	}

	bool Converter::HandleCamera(MSG& msg)
	{
		static int prevx = 0, prevy = 0;
		float sensitivity = 0.008f;

		switch (msg.message)
		{
		case WM_MOUSEWHEEL:
			if (0 > GET_WHEEL_DELTA_WPARAM(msg.wParam))
				m_camDistance *= 1.1f;
			else
				m_camDistance /= 1.1f;
			m_camera.position = mth::float3x3::Rotation(m_camera.rotation)*mth::float3(0.0f, 0.0f, -m_camDistance);
			return true;
		case WM_MOUSEMOVE:
			if (msg.wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				int x = GET_X_LPARAM(msg.lParam) - prevx;
				int y = GET_Y_LPARAM(msg.lParam) - prevy;

				if (msg.wParam & MK_RBUTTON)
				{
					m_entity->position += mth::float3x3::Rotation(m_camera.rotation) * mth::float3(x*sensitivity*m_camDistance*0.1f, -y * sensitivity*m_camDistance*0.1f, 0.0f);
				}
				else if (msg.wParam & MK_LBUTTON)
				{
					m_camera.LookUp(-y * sensitivity);
					m_camera.TurnRight(x*sensitivity);
					m_camera.position = mth::float3x3::Rotation(m_camera.rotation) * mth::float3(0.0f, 0.0f, -m_camDistance);
				}
				prevx += x;
				prevy += y;
				return true;
			}
			prevx = GET_X_LPARAM(msg.lParam);
			prevy = GET_Y_LPARAM(msg.lParam);
			return false;
		}
		return false;
	}

	Converter::P Converter::Create()
	{
		return std::make_shared<Converter>();
	}

	void Converter::Start()
	{
		ID3D11Device* device = m_graphics->getDevice();
		ID3D11DeviceContext* deviceContext = m_graphics->getDeviceContext();

		float width = 920.0f;
		float height = 720.0f;
		m_camera.Init(width / height);
		m_camDistance = 10.0f;
		m_camera.position.z = -m_camDistance;
		m_graphics->SetViewPort(width, height);

		DragAcceptFiles(m_window->getHWND(), true);

		m_renderer = gfw::SimpleRenderer::Create(m_graphics);
		m_renderer->AddEntity(m_entity = gfw::Entity::Create(gfw::Model::Create(device, L"Media/monkey.omd"),
			gfw::Texture::Create2D(device, L"Media/white.png"), gfw::Texture::Create2D(device, L"Media/normal.png")));
		//m_renderer->SetSky(gfw::SkyDome::Create(device, L"Media/skymap.dds"));
	}
	void Converter::Update(float deltaTime, float totalTime)
	{
		m_renderer->Render(m_graphics, m_camera);
		InvalidateRect(m_window->getHWND(), NULL, TRUE);
	}
	void Converter::HandleMessage(hcs::Input& input)
	{
		if (HandleCamera(input.getMSG()))
		{
			m_renderer->Render(m_graphics, m_camera);
			InvalidateRect(m_window->getHWND(), NULL, TRUE);
		}
		switch (input.getMSG().message)
		{
		case WM_DROPFILES:
			DropFileEvent((HDROP)input.getMSG().wParam);
			return;
		case WM_KEYDOWN:
			switch (input.getMSG().wParam)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				return;
			case 'E':
				m_modelManager.Export(gfw::SIL_POSITION | gfw::SIL_TEXCOORD | gfw::SIL_NORMAL | gfw::SIL_NORMALMAP);
				return;
			}
		}
	}
}