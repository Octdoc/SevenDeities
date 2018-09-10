#pragma once

#include "graphics/entity/gfw_entity.h"
#include "graphics/entity/gfw_skydome.h"
#include "graphics/entity/gfw_camera.h"

namespace gfw
{
	class Renderer
	{
		SHARED_ONLY(Renderer);

	protected:
		Renderer() = default;

		SkyDome::P m_sky;

		std::vector<Entity::P> m_entities;
		
	public:
		void SetSky(SkyDome::P sky);
		void AddEntity(Entity::P entity);
		void RemoveEntity(Entity::P entity);
		void ClearEntities();
		void RemoveSky();
		void Clear();

		virtual void Render(Graphics::P graphics, Camera& camera) = 0;
	};
}