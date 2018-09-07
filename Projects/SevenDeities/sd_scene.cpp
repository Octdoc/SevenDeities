#include "sd_scene.h"

#pragma comment(lib, "engine.lib")

namespace sd
{
	void SD_Scene::Test()
	{
	}

	void SD_Scene::Start()
	{
		Test();
		ID3D11Device* device = m_graphics->getDevice();
		ID3D11DeviceContext* deviceContext = m_graphics->getDeviceContext();

		m_controller.setTarget(&m_camera);
		m_camera.Init();
		m_camera.position = { 0.0f, 4.0f, -10.0f };

		//m_renderer = gfw::SimpleRenderer::Create(*m_graphics);
		m_renderer = gfw::ShadowRenderer::Create(*m_graphics);

		m_renderer->SetSky(gfw::SkyDome::Create(device, L"Media/skymap.dds"));

		auto monkey = gfw::Entity::Create(
			gfw::Model::Create(device, L"Media/smooth_monkey.omd"),
			gfw::Texture::Create2D(device, L"Media/white.png"),
			gfw::Texture::Create2D(device, L"Media/normal.png"));
		monkey->position.y = 2.0f;
		m_renderer->AddEntity(monkey);

		m_renderer->AddEntity(gfw::Entity::Create(
			gfw::Model::Create(device, L"Media/plain.omd"),
			gfw::Texture::Create2D(device, L"Media/green.png"),
			gfw::Texture::Create2D(device, L"Media/normal.png")));

	}
	void SD_Scene::Quit()
	{
	}
	void SD_Scene::Update(double deltaTime, double totalTime)
	{
		m_controller.Update_FirstPersonMode_Fly(*m_input, (float)deltaTime);
		m_input->ResetMouseDelta();
	}
	void SD_Scene::Render()
	{
		m_renderer->Render(*m_graphics, m_camera);
	}
	void SD_Scene::MessageHandler(MSG& message)
	{
		if (message.message == WM_KEYDOWN && message.wParam == VK_ESCAPE)
			PostQuitMessage(0);
	}
}