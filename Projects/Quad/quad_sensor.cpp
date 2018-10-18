#include "quad_sensor.h"

namespace quad
{
	gfw::Model::P Sensor::m_model;

	void Sensor::LoadModel(ID3D11Device* device)
	{
		m_model = gfw::Model::Create(device, L"Media/car/sensor.omd");
	}
	void Sensor::Init(mth::float3 position, mth::float3 rotation, gfw::Entity::P host)
	{
		Init(position, rotation, host, [](float dist, float shade)->float {return shade / dist; });
	}
	void Sensor::Init(mth::float3 position, mth::float3 rotation, gfw::Entity::P host, std::function<float(float dist, float shade)> equation)
	{
		m_entity = gfw::Entity::Create(m_model);
		m_entity->position = position;
		m_entity->rotation = rotation;
		m_entity->setRelativeTo(host);
		//m_entity->setColor({ 0.2f, 0.3f, 0.9f, 1.0f });
		m_equation = equation;
	}

	void Sensor::AddToRenderer(gfw::Renderer::P renderer)
	{
		renderer->AddEntity(m_entity);
	}

	void Sensor::Update(pfw::Collider::P enviroment[], float shades[], UINT count)
	{
		float mindist = 1000.0f;
		float materialBrightness = 0.0f;
		mth::float4x4 relativeMatrix = m_entity->getRelativeTo()->GetWorldMatrix();
		mth::Position pos;
		mth::float4x4 sensorMatrix = m_entity->GetWorldMatrix();
		for (UINT i = 0; i < count; i++)
		{
			for (mth::Triangle& tri : enviroment[i]->getMesh())
			{
				mth::Triangle t(enviroment[i]->GetWorldMatrix()*tri.vertices[0],
					enviroment[i]->GetWorldMatrix()*tri.vertices[1],
					enviroment[i]->GetWorldMatrix()*tri.vertices[2]);
				pos.position = relativeMatrix * m_entity->position;
				pos.rotation = m_entity->getRelativeTo()->rotation + m_entity->rotation;
				float dist = mth::DirectionalDistanceFromTriangle(pos, t);
				if (dist < mindist && dist > 0.1f)
				{
					mindist = dist;
					materialBrightness = shades[i];
				}
			}
			m_measurement = m_equation(mindist, materialBrightness);
			m_entity->setColor({ m_measurement, m_measurement, m_measurement, 1.0f });
		}
	}

	float Sensor::getMeasurement()
	{
		return m_measurement;
	}
	gfw::Entity::P Sensor::getEntity()
	{
		return m_entity;
	}

}
