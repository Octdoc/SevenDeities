#include "sd_scene.h"
#include <future>

#pragma comment(lib, "engine.lib")

namespace sd
{
	void SD_Scene::Start()
	{
		/*ID3D11Device* device = m_graphics->getDevice();
		ID3D11DeviceContext* deviceContext = m_graphics->getDeviceContext();

		m_controller.setTarget(&m_camera);
		m_camera.Init();
		m_camera.position = { 0.0f, 4.0f, -10.0f };

		//m_renderer = gfw::SimpleRenderer::Create(*m_graphics);
		m_renderer = gfw::ShadowRenderer::Create(m_graphics);

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
			gfw::Texture::Create2D(device, L"Media/normal.png")));*/

	}
	SD_Scene::P SD_Scene::Create()
	{
		return std::make_shared<SD_Scene>();
	}
	void SD_Scene::Quit()
	{
	}
	/*void SD_Scene::Update(float deltaTime, float totalTime)
	{
		m_controller.Update_FirstPersonMode_Fly(octdoc::Program::Instance().Input(), (float)deltaTime);
	}*/
	void SD_Scene::Render()
	{
		//m_renderer->Render(m_graphics, m_camera);
	}
	/*void SD_Scene::HandleMessage(hcs::Input& input)
	{
		if (input.getMSG().message == WM_KEYDOWN && input.getMSG().wParam == VK_ESCAPE)
			PostQuitMessage(0);
	}*/
}