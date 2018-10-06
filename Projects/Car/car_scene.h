#pragma once

#define USE_CURSOR 0

#include "car_renderer.h"
#include "car_car.h"

namespace car
{
	class Scene :public form::Scene
	{
		SHARED_ONLY(Scene);

		gfw::Controller m_controller;
		gfw::Camera m_camera;
		gfw::Renderer::P m_renderer;

		pfw::Collider::P m_pathMesh;
		pfw::Collider::P m_plainMesh;
		gfw::Entity::P m_plain;
		gfw::Entity::P m_path;
		std::array<Car, 4> m_cars;
		Car m_userCar;

		std::vector<pfw::Collider::P> m_enviromentColliders;
		std::vector<float> m_enviromentShades;

		std::vector<pfw::Collider::P> m_carColliders;
		std::vector<float> m_carShades;

		bool m_manual;
#if USE_CURSOR
		gfw::Entity::P m_cursor;
#endif

	private:
		Scene() = default;
		float Brightness(mth::float4 color);
		void DefaultCarPlacement();
		void SensorInputInit();
		void SensorUpdate();

	public:
		static Scene::P Create();

		virtual void Start() override;
		virtual void Update(float deltaTime, float totalTime) override;
		virtual void HandleMessage(hcs::Input& input) override;
	};
}