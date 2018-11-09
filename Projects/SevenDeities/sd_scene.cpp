#include "sd_scene.h"

namespace car
{
	void Scene::Start()
	{
		ID3D11Device* device = m_graphics->getDevice();
		ID3D11DeviceContext* deviceContext = m_graphics->getDeviceContext();

		m_controller.setTarget(&m_camera);
		m_camera.Init(m_graphics->getWidth() / m_graphics->getHeight());
		m_camera.position = { 0.0f, 4.0f, -10.0f };

		m_renderer = gfx::SimpleRenderer::Create(m_graphics);

		m_renderer->SetSky(gfx::SkyDome::Create(device, L"Media/skymap.dds"));

		gfx::ModelLoader ml;
		ml.CreateQuad({ 1.0f, 1.0f }, { 2.0f, 2.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, gfx::SIL_POSITION | gfx::SIL_TEXCOORD | gfx::SIL_NORMAL | gfx::SIL_NORMALMAP);
		//ml.Transform(mth::float4x4::Translation(0.0f, 0.0f, 1.0f)*mth::float4x4::RotationX(-mth::pi*0.5f));
		auto monkey = gfx::Entity::Create(
			//gfw::Model::Create(device, L"Media/smooth_monkey.omd"),
			gfx::Model::Create(device, ml),
			gfx::Texture::Create2D(device, L"Media/white.png"),
			gfx::Texture::Create2D(device, L"Media/testn.png"));
		monkey->position.y = 2.0f;
		m_renderer->AddEntity(monkey);

		m_renderer->AddEntity(gfx::Entity::Create(
			gfx::Model::Create(device, L"Media/plain.omd"),
			gfx::Texture::Create2D(device, L"Media/green.png"),
			gfx::Texture::Create2D(device, L"Media/testn.png")));
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