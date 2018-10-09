#include "quad_scene.h"

#pragma comment(lib, "engine.lib")

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
		//m_renderer = gfw::ColorShadowRenderer::Create(m_graphics);
		m_renderer = gfw::ColorRenderer::Create(m_graphics);
		//m_renderer->SetSky(gfw::SkyDome::Create(device, L"Media/skymap.dds"));
		m_graphics->setClearColor(0.5f, 0.75f, 0.96f);

		m_plain = gfw::Entity::Create(gfw::Model::Create(device, L"Media/quad/plain.omd"));
		m_plain->setColor({ 0.2f, 1.0f, 0.35f, 1.0f });
		m_renderer->AddEntity(m_plain);

		m_quad.Init(device);
		m_renderer->AddEntity(m_quad.getEntity());

		m_walk.Init(&m_quad);
	}
	void Scene::Update(float deltaTime, float totalTime)
	{
		hcs::Input& input = octdoc::Program::Instance().Input();
		m_controller.Update_FirstPersonMode_Fly(octdoc::Program::Instance().Input(), deltaTime);

		float speed = 1.0f;
		if (input.isPressed(VK_PRIOR))
			m_quad.getEntity()->position.y += speed * deltaTime;
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

		UINT legid = 0;
		deltaTime *= 0.3f;
		if (input.isPressed('1'))
		{
			m_quad.getLegRF().MovePosition(mth::float3(deltaTime, 0.0f, 0.0f));
			m_quad.getLegRB().MovePosition(mth::float3(deltaTime, 0.0f, 0.0f));
			m_quad.getLegLF().MovePosition(mth::float3(-deltaTime, 0.0f, 0.0f));
			m_quad.getLegLB().MovePosition(mth::float3(-deltaTime, 0.0f, 0.0f));
		}
		if (input.isPressed('3'))
		{
			m_quad.getLegRF().MovePosition(mth::float3( 0.0f, deltaTime,0.0f));
			m_quad.getLegRB().MovePosition(mth::float3( 0.0f, deltaTime,0.0f));
			m_quad.getLegLF().MovePosition(mth::float3( 0.0f, deltaTime,0.0f));
			m_quad.getLegLB().MovePosition(mth::float3( 0.0f, deltaTime,0.0f));
		}
		if (input.isPressed('5'))
		{
			m_quad.getLegRF().MovePosition(mth::float3(0.0f, 0.0f, -deltaTime));
			m_quad.getLegRB().MovePosition(mth::float3(0.0f, 0.0f, deltaTime));
			m_quad.getLegLF().MovePosition(mth::float3(0.0f, 0.0f, -deltaTime));
			m_quad.getLegLB().MovePosition(mth::float3(0.0f, 0.0f, deltaTime));
		}
		if (input.isPressed('2'))
		{
			m_quad.getLegRF().MovePosition(mth::float3(-deltaTime, 0.0f, 0.0f));
			m_quad.getLegRB().MovePosition(mth::float3(-deltaTime, 0.0f, 0.0f));
			m_quad.getLegLF().MovePosition(mth::float3(deltaTime, 0.0f, 0.0f));
			m_quad.getLegLB().MovePosition(mth::float3(deltaTime, 0.0f, 0.0f));
		}
		if (input.isPressed('4'))
		{
			m_quad.getLegRF().MovePosition(mth::float3(0.0f, -deltaTime, 0.0f));
			m_quad.getLegRB().MovePosition(mth::float3(0.0f, -deltaTime, 0.0f));
			m_quad.getLegLF().MovePosition(mth::float3(0.0f, -deltaTime, 0.0f));
			m_quad.getLegLB().MovePosition(mth::float3(0.0f, -deltaTime, 0.0f));
		}
		if (input.isPressed('6'))
		{
			m_quad.getLegRF().MovePosition(mth::float3(0.0f, 0.0f, deltaTime));
			m_quad.getLegRB().MovePosition(mth::float3(0.0f, 0.0f, -deltaTime));
			m_quad.getLegLF().MovePosition(mth::float3(0.0f, 0.0f, deltaTime));
			m_quad.getLegLB().MovePosition(mth::float3(0.0f, 0.0f, -deltaTime));
		}
		m_walk.Update(deltaTime);
		mth::float3 pos = m_quad.getLeg(legid).getPosition();
		m_labels[0]->setText(std::to_wstring(pos.x).c_str());
		m_labels[1]->setText(std::to_wstring(pos.y).c_str());
		m_labels[2]->setText(std::to_wstring(pos.z).c_str());

		//m_quad.getLegRF().setPosition({ 0.9f, -0.15f, 1.2f });

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
				/*case 'L':
					std::dynamic_pointer_cast<gfw::ShadowRenderer>(m_renderer)->getLight().position = m_camera.position;
					std::dynamic_pointer_cast<gfw::ShadowRenderer>(m_renderer)->getLight().rotation = m_camera.rotation;
					break;*/
			}
		}
	}
}