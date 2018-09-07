#pragma once

#include "graphics/entity/gfw_entity.h"
#include "graphics/entity/gfw_skydome.h"
#include "graphics/entity/gfw_camera.h"

namespace gfw
{
	class Renderer
	{
	protected:
		std::shared_ptr<SkyDome> m_sky;

		std::vector<std::shared_ptr<Entity>> m_entities;
		
	public:
		void SetSky(std::shared_ptr<SkyDome> sky);
		void AddEntity(std::shared_ptr<Entity> entity);
		void RemoveEntity(std::shared_ptr<Entity> entity);
		void ClearEntities();
		void RemoveSky();
		void Clear();

		virtual void Render(Graphics& graphics, Camera& camera) = 0;
	};
}