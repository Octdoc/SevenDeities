#include "quad_scene.h"

#pragma comment(lib, "engine.lib")

namespace quad
{
	Scene::P Scene::Create()
	{
		return std::make_shared<Scene>();
	}
	void Scene::Start()
	{
		ID3D11Device* device = m_graphics->getDevice();
		ID3D11DeviceContext* deviceContext = m_graphics->getDeviceContext();

		m_controller.setTarget(&m_camera);
		m_camera.Init(m_graphics->getWidth() / m_graphics->getHeight());
		m_camera.position = { 2.0f, 4.0f, -4.0f };
		m_camera.rotation = { 0.67f, -0.5f, 0.0f };
		m_renderer = gfw::ColorRenderer::Create(m_graphics);
		//m_renderer->SetSky(gfw::SkyDome::Create(device, L"Media/skymap.dds"));
		m_graphics->setClearColor(0.5f, 0.75f, 0.96f);

		m_plain = gfw::Entity::Create(gfw::Model::Create(device, L"Media/quad/plain.omd"));
		m_plain->setColor({ 0.2f, 1.0f, 0.35f, 1.0f });
		m_renderer->AddEntity(m_plain);

		m_quad.Init(device);
		m_renderer->AddEntity(m_quad.getEntity());
		for (Leg& leg : m_quad.getLegs())
			leg.setJointStates({ 0.0f, -mth::pi*0.25f, mth::pi*0.25f });
	}
	void Scene::Update(float deltaTime, float totalTime)
	{
		hcs::Input& input = octdoc::Program::Instance().Input();
		m_controller.Update_FirstPersonMode_Fly(octdoc::Program::Instance().Input(), deltaTime);
		m_renderer->Render(m_graphics, m_camera);

		float speed = 1.0f;
		if (input.isPressed(VK_PRIOR))
			m_quad.getEntity()->position.y += speed*deltaTime;
		if (input.isPressed(VK_NEXT))
			m_quad.getEntity()->position.y -= speed * deltaTime;
		if (input.isPressed(VK_RIGHT))
			m_quad.getEntity()->position.x += speed * deltaTime;
		if (input.isPressed(VK_LEFT))
			m_quad.getEntity()->position.x -= speed * deltaTime;
		if (input.isPressed(VK_UP))
			m_quad.getEntity()->position.z += speed * deltaTime;
		if (input.isPressed(VK_DOWN))
			m_quad.getEntity()->position.z -= speed * deltaTime;
	}
	void Scene::HandleMessage(hcs::Input& input)
	{
		if (input.getMSG().hwnd == m_window->getHWND())
		{
			m_controller.HandleMouseMove(input);
		}
		if (input.getMSG().message == WM_KEYDOWN)
		{
			switch (input.getMSG().wParam)
			{
			case  VK_ESCAPE:
				PostQuitMessage(0);
				break;
			/*case 'L':
				std::dynamic_pointer_cast<gfw::ShadowRenderer>(m_renderer)->getLight().position = m_camera.position;
				std::dynamic_pointer_cast<gfw::ShadowRenderer>(m_renderer)->getLight().rotation = m_camera.rotation;
				break;*/
			}
		}
	}
}