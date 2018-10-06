#include "car_car.h"

namespace car
{
	gfw::Model::P Car::m_model;
	gfw::ModelLoader Car::m_modelLoader;

	void Car::Init(mth::float3 position, mth::float3 rotation, mth::float4 color)
	{
		m_entity = gfw::Entity::Create(m_model);
		m_entity->setColor(color);
		m_entity->position = position;
		m_entity->rotation = rotation;

		m_collider = pfw::Collider::Create(m_modelLoader);

		m_speed = 10.0f;
		m_steering = 0.5f;

		mth::float3 pos = { -0.25f, 0.15f, -0.7f };
		for (auto& s : m_lightSensors)
		{
			s.Init(pos, { mth::pi / 2.0f, 0.0f, 0.0f }, m_entity);
			pos.x += 0.1f;
		}
		pos = { -0.25, 0.35, -0.8 };
		for (auto& s : m_distanceSensors)
		{
			s.Init(pos, { 0.0f, mth::pi, 0.0f }, m_entity,
				[](float dist, float shade)->float {return dist * 0.001f; });
			pos.x += 0.5f;
		}
	}

	void Car::AddToRenderer(gfw::Renderer::P renderer)
	{
		renderer->AddEntity(m_entity);
		for (auto& s : m_lightSensors)
			s.AddToRenderer(renderer);
		for (auto& s : m_distanceSensors)
			s.AddToRenderer(renderer);
	}

	void Car::LoadModel(ID3D11Device* device)
	{
		m_modelLoader.LoadModel(L"Media/car.omd");
		m_model = gfw::Model::Create(device, m_modelLoader);
	}

	gfw::Entity::P Car::getEntity()
	{
		return m_entity;
	}
	pfw::Collider::P Car::getCollider()
	{
		return m_collider;
	}
	float Car::getSpeed()
	{
		return m_speed;
	}
	void Car::setSpeed(float speed)
	{
		m_speed = speed;
	}
	float Car::getSteering()
	{
		return m_steering;
	}
	void Car::setSteering(float steering)
	{
		m_steering = steering;
	}

	Sensor& Car::getLightSensor(UINT index)
	{
		return m_lightSensors[index];
	}

	Sensor& Car::getDistanceSensor(UINT index)
	{
		return m_distanceSensors[index];
	}

	void Car::UpdateLightSensors(pfw::Collider::P enviroment[], float shades[], UINT count)
	{
		for (auto& s : m_lightSensors)
			s.Update(enviroment, shades, count);
	}
	void Car::UpdateDistanceSensors(pfw::Collider::P enviroment[], float shades[], UINT count)
	{
		for (auto& s : m_distanceSensors)
			s.Update(enviroment, shades, count);
	}

	void Car::Control_Auto(float deltaTime)
	{
		if (m_distanceSensors[0].getMeasurement() < 0.002f ||
			m_distanceSensors[1].getMeasurement() < 0.002f)
			return;
		float distanceTaken = 0.0f;
		m_entity->MoveForward(-m_speed * deltaTime);
		distanceTaken += m_speed * deltaTime;
		if (m_lightSensors[1].getMeasurement() < 0.7f)
			m_entity->TurnRight(distanceTaken*m_steering);
		if (m_lightSensors[4].getMeasurement() < 0.7f)
			m_entity->TurnLeft(distanceTaken*m_steering);

		m_collider->position = m_entity->position;
		m_collider->rotation = m_entity->rotation;
	}

	void Car::Control_Manual(float deltaTime)
	{
		float distanceTaken = 0.0f;
		auto input = octdoc::Program::Instance().Input();
		if (input.isPressed(VK_UP))
		{
			m_entity->MoveForward(-m_speed * deltaTime);
			distanceTaken += m_speed * deltaTime;
		}
		if (input.isPressed(VK_DOWN))
		{
			m_entity->MoveBackward(-m_speed * deltaTime);
			distanceTaken -= m_speed * deltaTime;
		}
		if (input.isPressed(VK_RIGHT))
			m_entity->TurnRight(distanceTaken*m_steering);
		if (input.isPressed(VK_LEFT))
			m_entity->TurnLeft(distanceTaken*m_steering);
		if (input.isPressed(VK_PRIOR))
			m_entity->MoveUp(m_speed*deltaTime);
		if (input.isPressed(VK_NEXT))
			m_entity->MoveDown(m_speed*deltaTime);

		m_collider->position = m_entity->position;
		m_collider->rotation = m_entity->rotation;
	}

}
