#include "sd_scene.h"

#pragma comment(lib, "engine.lib")

namespace car
{
	void Scene::Start()
	{
		ID3D11Device* device = m_graphics->getDevice();
		ID3D11DeviceContext* deviceContext = m_graphics->getDeviceContext();

		m_controller.setTarget(&m_camera);
		m_camera.Init(m_graphics->getWidth() / m_graphics->getHeight());
		m_camera.position = { 0.0f, 4.0f, -10.0f };

		m_renderer = gfw::SimpleRenderer::Create(m_graphics);

		m_renderer->SetSky(gfw::SkyDome::Create(device, L"Media/skymap.dds"));

		gfw::ModelLoader ml;
		ml.CreateQuad({ 1.0f, 1.0f }, { 2.0f, 2.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, gfw::SIL_POSITION | gfw::SIL_TEXCOORD | gfw::SIL_NORMAL | gfw::SIL_NORMALMAP);
		//ml.Transform(mth::float4x4::Translation(0.0f, 0.0f, 1.0f)*mth::float4x4::RotationX(-mth::pi*0.5f));
		auto monkey = gfw::Entity::Create(
			//gfw::Model::Create(device, L"Media/smooth_monkey.omd"),
			gfw::Model::Create(device, ml),
			gfw::Texture::Create2D(device, L"Media/white.png"),
			gfw::Texture::Create2D(device, L"Media/testn.png"));
		monkey->position.y = 2.0f;
		m_renderer->AddEntity(monkey);

		m_renderer->AddEntity(gfw::Entity::Create(
			gfw::Model::Create(device, L"Media/plain.omd"),
			gfw::Texture::Create2D(device, L"Media/green.png"),
			gfw::Texture::Create2D(device, L"Media/testn.png")));
	}
	Scene::P Scene::Create()
	{
		return std::make_shared<Scene>();
	}
	void Scene::Frame(float deltaTime)
	{
		m_controller.Update_FirstPersonMode_Fly(octdoc::Program::Instance().Input(), deltaTime);
		m_renderer->Render(m_graphics, m_camera);
	}
	void Scene::MessageHandler(hcs::Input& input)
	{
		m_controller.HandleMouseMove(input);
		if (input.getMSG().message == WM_KEYDOWN && input.getMSG().wParam == VK_ESCAPE)
			PostQuitMessage(0);
	}
}