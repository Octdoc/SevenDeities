#pragma once

#include "car_sensor.h"
#include <array>

namespace car
{
	class Car
	{
		static gfw::Model::P m_model;
		static gfw::ModelLoader m_modelLoader;

		gfw::Entity::P m_entity;
		pfw::Collider::P m_collider;
		std::array<Sensor, 6> m_lightSensors;
		std::array<Sensor, 2> m_distanceSensors;
		float m_speed;
		float m_steering;

	public:
		Car() = default;
		void Init(mth::float3 position, mth::float3 rotation, mth::float4 = { 1.0f, 0.4f, 0.2f, 1.0f });
		void AddToRenderer(gfw::Renderer::P renderer);
		static void LoadModel(ID3D11Device* device);
		gfw::Entity::P getEntity();
		pfw::Collider::P getCollider();
		float getSpeed();
		void setSpeed(float speed);
		float getSteering();
		void setSteering(float steering);

		Sensor& getLightSensor(UINT index);
		Sensor& getDistanceSensor(UINT index);

		void UpdateLightSensors(pfw::Collider::P enviroment[], float shades[], UINT count);
		void UpdateDistanceSensors(pfw::Collider::P enviroment[], float shades[], UINT count);
		void Control_Auto(float deltaTime);
		void Control_Manual(float deltaTime);
	};
}