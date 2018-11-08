#include "quad_scene.h"

#pragma comment(lib, "engine.lib")

#define USE_SHADOW 0


namespace quad
{
	Scene::P Scene::Create()
	{
		return std::make_shared<Scene>();
	}
	Scene::P Scene::Create(form::Label::P labels[])
	{
		auto scene = std::make_shared<Scene>();
		scene->m_labels[0] = labels[0];
		scene->m_labels[1] = labels[1];
		scene->m_labels[2] = labels[2];
		return scene;
	}
	void Scene::Start()
	{
		ID3D11Device* device = m_graphics->getDevice();
		ID3D11DeviceContext* deviceContext = m_graphics->getDeviceContext();

		m_controller.setTarget(&m_camera);
		m_camera.Init(m_graphics->getWidth() / m_graphics->getHeight());
		m_camera.position = { 2.0f, 4.0f, -4.0f };
		m_camera.rotation = { 0.67f, -0.5f, 0.0f };
#if USE_SHADOW
		m_renderer = gfw::ColorShadowRenderer::Create(m_graphics);
#else
		m_renderer = gfw::ColorRenderer::Create(m_graphics);
#endif
		//m_renderer->SetSky(gfw::SkyDome::Create(device, L"Media/skymap.dds"));
		m_graphics->setClearColor(0.5f, 0.75f, 0.96f);

		gfw::ModelLoader ml;
		ml.LoadModel(L"Media/quad/plain.omd");
		m_plain = gfw::Entity::Create(gfw::Model::Create(device, ml));
		m_plain->setColor({ 0.2f, 1.0f, 0.35f, 1.0f });
		m_renderer->AddEntity(m_plain);
		m_enviroment = pfw::Collider::Create(ml);

		Sensor::LoadModel(device);
		m_quad.Init(device);
		m_renderer->AddEntity(m_quad.getEntity());
		m_quad.getSensor().getEntity()->Hide();

		m_walk.Init(&m_quad);
		m_running = true;
	}
	void Scene::Update(float deltaTime, float totalTime)
	{
		hcs::Input& input = octdoc::Program::Instance().Input();
		m_controller.Update_FirstPersonMode_Fly(input, deltaTime);

		if (m_running)	m_walk.Update(deltaTime);
		mth::float3 pos = m_quad.getLeg(LID_RF).getPosition();
		pos = m_quad.getEntity()->position;
		pos.y = m_quad.getSensor().getMeasurement();
		m_labels[0]->setText(std::to_wstring(pos.x).c_str());
		m_labels[1]->setText(std::to_wstring(pos.y).c_str());
		m_labels[2]->setText(std::to_wstring(pos.z).c_str());

		if (input.isPressed(VK_UP))
			m_quad.getEntity()->MoveForward(deltaTime*5.0f);
		if (input.isPressed(VK_DOWN))
			m_quad.getEntity()->MoveBackward(deltaTime*5.0f);
		if (input.isPressed(VK_LEFT))
			m_quad.getEntity()->TurnLeft(deltaTime*1.8f);
		if (input.isPressed(VK_RIGHT))
			m_quad.getEntity()->TurnRight(deltaTime*1.8f);

#if USE_SHADOW
		std::dynamic_pointer_cast<gfw::ShadowRenderer>(m_renderer)->getLight().position = m_quad.getEntity()->position + mth::float3(-0.3f, 6.0f, -0.4f);
		std::dynamic_pointer_cast<gfw::ShadowRenderer>(m_renderer)->getLight().rotation = { mth::pi*0.5f, 0.0f, 0.0f };
#endif
		pfw::Collider::P env[1] = { m_enviroment };
		float shades[1] = { 1.0f };
		m_quad.getSensor().Update(env, shades, 1);

		m_renderer->Render(m_graphics, m_camera);
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
			case 'R':
				m_walk.Init(&m_quad);
				break;
			case 'T':
				m_running = !m_running;
				break;
#if USE_SHADOW
			case 'L':
				std::dynamic_pointer_cast<gfw::ShadowRenderer>(m_renderer)->getLight().position = m_camera.position;
				std::dynamic_pointer_cast<gfw::ShadowRenderer>(m_renderer)->getLight().rotation = m_camera.rotation;
				break;
#endif
			}
		}
	}
}