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
		m_physicsArea->AddCollider(pfw::Collider::Create(monkeyHB, mth::float3(0.0f, 2.0f, 0.0f)));
		m_physicsArea->AddCollider(pfw::Collider::Create(floor));
		m_physicsArea->gravity = 0.0f;

	}
	void Test_Scene::Update(float deltaTime, float totalTime)
	{
		static double prevTime = 0.0;
		static double tTime = 0.0;
		static UINT frameCount = 0;
		tTime += deltaTime;
		frameCount++;
		if (tTime - prevTime > 1.0)
		{
			//std::wcout << L"Total Time: " << tTime << std::endl << L"Previous Time: " << prevTime << std::endl << std::endl;
			SetWindowText(m_window->getHWND(), (std::wstring(L"Physics test, Fps: ") + std::to_wstring(frameCount)).c_str());
			frameCount = 0;
			prevTime = tTime;
		}

		m_controller.Update_FirstPersonMode_Fly(Program::Instance().Input(), (float)deltaTime);

		m_phyPlayer->Control_FreeMove(Program::Instance().Input());
		m_physicsArea->Update((float)deltaTime);
		m_gfxPlayer->position = m_phyPlayer->position;

		m_renderer->Render(m_graphics, m_camera);
	}
	void Test_Scene::HandleMessage(hcs::Input& input)
	{
		if (input.getMSG().message == WM_KEYDOWN && input.getMSG().wParam == VK_ESCAPE)
			PostQuitMessage(0);
	}
}