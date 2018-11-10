#pragma once

#include "octdoc.h"
#include <functional>

using namespace octdoc;

namespace quad
{
	class Sensor
	{
		SHARED_TOO(Sensor)

		static gfx::Model::P m_model;

		gfx::Entity::P m_entity;
		float m_measurement;
		std::function<float(float dist, float shade)> m_equation;

	public:
		Sensor() = default;
		static void LoadModel(ID3D11Device* device);
		void Init(mth::float3 position, mth::float3 rotation, gfx::Entity::P host);
		void Init(mth::float3 position, mth::float3 rotation, gfx::Entity::P host, std::function<float(float dist, float shade)> equation);
		void AddToRenderer(gfx::Renderer::P renderer);

		void Update(physx::Collider::P enviroment[], float shades[], UINT count);

		float getMeasurement();
		gfx::Entity::P getEntity();
	};
}