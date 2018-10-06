#include "test_scene.h"
#include <iostream>

#pragma comment(lib, "engine.lib")

using namespace octdoc;

namespace test
{
	Test_Scene::P Test_Scene::Create()
	{
		return std::make_shared<Test_Scene>();
	}

	void Test_Scene::Start()
	{
		ID3D11Device* device = m_graphics->getDevice();
		ID3D11DeviceContext* deviceContext = m_graphics->getDeviceContext();

		m_controller.setTarget(&m_camera);
		m_camera.Init();
		m_camera.position = { 0.0f, 4.0f, -10.0f };

		//m_renderer = gfw::SimpleRenderer::Create(m_graphics);
		m_renderer = gfw::ShadowRenderer::Create(m_graphics);

		m_renderer->SetSky(gfw::SkyDome::Create(device, L"Media/skymap.dds"));

		auto normapmap = gfw::Texture::Create2D(device, L"Media/normal.png");

		auto monkey = gfw::Entity::Create(
			gfw::Model::Create(device, L"Media/smooth_monkey.omd"),
			gfw::Texture::Create2D(device, L"Media/white.png"),
			normapmap);
		monkey->position.y = 2.0f;
		m_renderer->AddEntity(monkey);

		gfw::ModelLoader floor;
		floor.LoadModel(L"Media/plain.omd");
		m_renderer->AddEntity(gfw::Entity::Create(
			gfw::Model::Create(device, floor),
			gfw::Texture::Create2D(device, L"Media/green.png"),
			normapmap));

		gfw::ModelLoader rod;
		rod.CreateQuad({ -0.002f, -0.002f }, { 0.004f, 0.004f }, 1.0f, gfw::SIL_POSITION | gfw::SIL_TEXCOORD | gfw::SIL_NORMAL | gfw::SIL_NORMALMAP);
		m_rod = gfw::Entity::Create(gfw::Model::Create(device, rod),
			gfw::Texture::Create2D(device, L"Media/black.png"),
			gfw::Texture::Create2D(device, L"Media/normal.png"));
		m_renderer->AddEntity(m_rod);

		m_gfxPlayer = gfw::Entity::Create(gfw::Model::Create(device, L"Media/sphere.omd"),
			gfw::Texture::Create2D(device, L"Media/red.png"),
			normapmap);
		m_renderer->AddEntity(m_gfxPlayer);

		m_physicsArea = pfw::CollisionArea::Create();

		m_phyPlayer = pfw::Player::Create();
		m_phyPlayer->position = { -2.0f, 1.5f, -3.0f };
		m_phyPlayer->scale = { 0.35f, 0.9f, 0.35f };
		m_gfxPlayer->scale = { 0.35f, 0.9f, 0.35f };
		m_physicsArea->setPlayer(m_phyPlayer);

		gfw::ModelLoader monkeyHB;
		monkeyHB.LoadModel(L"Media/monkey.omd");
		m_physicsArea->AddCollider(m_phyMonkey = pfw::Collider::Create(monkeyHB, mth::float3(0.0f, 2.0f, 0.0f)));
		m_physicsArea->AddCollider(m_phyFloor = pfw::Collider::Create(floor));
		m_physicsArea->gravity = 0.0f;

	}
	void Test_Scene::Update(float deltaTime, float totalTime)
	{
		pfw::Collider::P obj;
		float mindist = 1000.0f;
		for (mth::Triangle t : m_phyMonkey->getMesh())
		{
			mth::Triangle tri(t.vertices[0] + m_phyMonkey->position, t.vertices[1] + m_phyMonkey->position, t.vertices[2] + m_phyMonkey->position);
			float d = mth::DirectionalDistanceFromTriangle(m_camera, tri);
			if (!isnan(d))
			{
				if (d < mindist)
				{
					mindist = d;
					obj = m_phyMonkey;
				}
			}
		}
		for (mth::Triangle t : m_phyFloor->getMesh())
		{
			mth::Triangle tri(t.vertices[0] + m_phyFloor->position, t.vertices[1] + m_phyFloor->position, t.vertices[2] + m_phyFloor->position);
			float d = mth::DirectionalDistanceFromTriangle(m_camera, tri);
			if (!isnan(d))
			{
				if (d < mindist)
				{
					mindist = d;
					obj = m_phyFloor;
				}
			}
		}
		if (obj)
			SetWindowText(m_window->getHWND(), obj==m_phyMonkey ? L"Monkey" : L"Floor");
		else
			SetWindowText(m_window->getHWND(), L"Nothing");
		
		m_controller.Update_FirstPersonMode_Fly(Program::Instance().Input(), (float)deltaTime);

		m_phyPlayer->Control_FreeMove(Program::Instance().Input());
		m_physicsArea->Update((float)deltaTime);
		m_gfxPlayer->position = m_phyPlayer->position;

		m_rod->position = m_camera.position;
		m_rod->rotation = m_camera.rotation;

		m_renderer->Render(m_graphics, m_camera);
	}
	void Test_Scene::HandleMessage(hcs::Input& input)
	{
		m_controller.HandleMouseMove(input);
		if (input.getMSG().message == WM_KEYDOWN && input.getMSG().wParam == VK_ESCAPE)
			PostQuitMessage(0);
	}
}