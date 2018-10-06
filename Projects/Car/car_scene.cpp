#include "car_scene.h"
#include <thread>

#pragma comment(lib, "engine.lib")

namespace car
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
		m_camera.position = { 0.0f, 4.0f, -10.0f };
		m_renderer = Renderer::Create(m_graphics);
		//m_renderer->SetSky(gfw::SkyDome::Create(device, L"Media/skymap.dds"));
		m_graphics->setClearColor(0.5f, 0.75f, 0.96f);
		m_manual = true;

#if USE_CURSOR
		gfw::ModelLoader cursor;
		cursor.CreateQuad({ -0.0002f, -0.0002f }, { 0.0004f, 0.0004f }, 0.11f, gfw::SIL_POSITION | gfw::SIL_NORMAL);
		m_cursor = gfw::Entity::Create(gfw::Model::Create(device, cursor));
		m_renderer->AddEntity(m_cursor);
#endif

		gfw::ModelLoader plainModel(L"Media/plain.omd");
		gfw::ModelLoader pathModel(L"Media/path.omd");
		m_plain = gfw::Entity::Create(gfw::Model::Create(device, plainModel));
		m_plain->setColor({ 0.2f, 1.0f, 0.35f, 1.0f });
		m_plainMesh = pfw::Collider::Create(plainModel);
		m_path = gfw::Entity::Create(gfw::Model::Create(device, pathModel));
		m_path->setColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		m_path->position.y += 0.01f;
		m_pathMesh = pfw::Collider::Create(pathModel);
		m_renderer->AddEntity(m_path);
		m_renderer->AddEntity(m_plain);


		Sensor::LoadModel(device);
		Car::LoadModel(device);
		for (auto& c : m_cars)
		{
			c.Init(0.0f, 0.0f);
			c.AddToRenderer(m_renderer);
		}
		m_userCar.Init(0.0f, 0.0f, { 0.2f, 0.4f, 1.0f, 1.0f });
		m_userCar.setSpeed(4.0f);
		m_userCar.AddToRenderer(m_renderer);
		SensorInputInit();

		std::thread th([this]() {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			DefaultCarPlacement(); });
		th.detach();
	}
	float Scene::Brightness(mth::float4 color)
	{
		return 0.299f*color.x + 0.587f*color.y + 0.114f*color.z;
	}
	void Scene::DefaultCarPlacement()
	{
		mth::float3 pos;
		for (auto& c : m_cars)
		{
			c.getEntity()->position = pos;
			c.getEntity()->rotation = 0.0f;
			pos.z += 3.0f;
		}
		m_userCar.getEntity()->position = { 3.0f, 0.0f, 0.0f };
		m_userCar.getEntity()->rotation = { 0.0f, -mth::pi / 2.0f, 0.0f };
	}
	void Scene::SensorInputInit()
	{
		m_enviromentColliders.push_back(m_pathMesh);
		m_enviromentColliders.push_back(m_plainMesh);
		m_enviromentShades.push_back(Brightness(m_path->getColor()));
		m_enviromentShades.push_back(Brightness(m_plain->getColor()));

		for (auto& c : m_cars)
		{
			m_carColliders.push_back(c.getCollider());
			m_carShades.push_back(Brightness(c.getEntity()->getColor()));
		}
		m_carColliders.push_back(m_userCar.getCollider());
		m_carShades.push_back(Brightness(m_userCar.getEntity()->getColor()));
	}
	void Scene::SensorUpdate()
	{
		for (auto& c : m_cars)
		{
			c.UpdateLightSensors(m_enviromentColliders.data(), m_enviromentShades.data(), m_enviromentColliders.size());
			c.UpdateDistanceSensors(m_carColliders.data(), m_carShades.data(), m_carColliders.size());
		}
		m_userCar.UpdateLightSensors(m_enviromentColliders.data(), m_enviromentShades.data(), m_enviromentColliders.size());
		m_userCar.UpdateDistanceSensors(m_carColliders.data(), m_carShades.data(), m_carColliders.size());
	}
	void Scene::Update(float deltaTime, float totalTime)
	{
		SensorUpdate();
		for (auto& c : m_cars)
			c.Control_Auto(deltaTime);
		m_manual ? m_userCar.Control_Manual(deltaTime) : m_userCar.Control_Auto(deltaTime);
		m_controller.Update_FirstPersonMode_Fly(octdoc::Program::Instance().Input(), deltaTime);
		if (octdoc::Program::Instance().Input().isPressed('C'))
		{
			m_camera.position = m_userCar.getDistanceSensor(0).getEntity()->position;
			m_camera.rotation = m_userCar.getDistanceSensor(0).getEntity()->rotation;
		}
#if USE_CURSOR
		m_cursor->position = m_camera.position;
		m_cursor->rotation = m_camera.rotation;
		float mindist = 1000.0f;
		m_cursor->setColor(0.0f);
		for (UINT i = 0; i < m_carColliders.size(); i++)
		{
			for (mth::Triangle& tri : m_carColliders[i]->getMesh())
			{
				mth::Triangle t(m_carColliders[i]->GetWorldMatrix()*tri.vertices[0],
					m_carColliders[i]->GetWorldMatrix()*tri.vertices[1],
					m_carColliders[i]->GetWorldMatrix()*tri.vertices[2]);
				float dist = mth::DirectionalDistanceFromTriangle(m_camera, t);
				if (dist < mindist)
				{
					mindist = dist;
					m_cursor->setColor(i == m_cars.size() ?
						m_userCar.getEntity()->getColor() :
						m_cars[0].getEntity()->getColor());
				}
			}
		}
#endif
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
				DefaultCarPlacement();
				break;
			case 'M':
				m_manual = !m_manual;
				break;
			}
		}
	}
}