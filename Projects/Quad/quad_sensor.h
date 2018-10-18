#pragma once

#include "octdoc.h"
#include <functional>

namespace quad
{
	class Sensor
	{
		SHARED_TOO(Sensor);

		static gfw::Model::P m_model;

		gfw::Entity::P m_entity;
		float m_measurement;
		std::function<float(float dist, float shade)> m_equation;

	public:
		Sensor() = default;
		static void LoadModel(ID3D11Device* device);
		void Init(mth::float3 position, mth::float3 rotation, gfw::Entity::P host);
		void Init(mth::float3 position, mth::float3 rotation, gfw::Entity::P host, std::function<float(float dist, float shade)> equation);
		void AddToRenderer(gfw::Renderer::P renderer);

		void Update(pfw::Collider::P enviroment[], float shades[], UINT count);

		float getMeasurement();
		gfw::Entity::P getEntity();
	};
}