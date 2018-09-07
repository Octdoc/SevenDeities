#include "cvt_converter.h"

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
				m_renderer->AddEntity(m_modelManager.CreateEntity(*m_graphics));
			}
			catch (hcs::Exception& e)
			{
				std::wcout << e.Caption() << std::endl << e.Description() << std::endl;
			}
		}
	}

	void Converter::Start()
	{
		ID3D11Device* device = m_graphics->getDevice();
		ID3D11DeviceContext* deviceContext = m_graphics->getDeviceContext();

		m_controller.setTarget(&m_camera);
		m_camera.Init();
		m_camera.position = { 0.0f, 4.0f, -10.0f };

		DragAcceptFiles(m_window->getHWND(), true);

		m_renderer = gfw::SimpleRenderer::Create(*m_graphics);
	}
	void Converter::Quit()
	{
	}
	void Converter::Update(double deltaTime, double totalTime)
	{
		m_controller.Update_FirstPersonMode_Fly(*m_input, (float)deltaTime);
		m_input->ResetMouseDelta();
	}
	void Converter::Render()
	{
		m_renderer->Render(*m_graphics, m_camera);
	}
	void Converter::MessageHandler(MSG& message)
	{
		switch (message.message)
		{
		case WM_DROPFILES:
			DropFileEvent((HDROP)message.wParam);
			return;
		case WM_KEYDOWN:
			switch (message.wParam)
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