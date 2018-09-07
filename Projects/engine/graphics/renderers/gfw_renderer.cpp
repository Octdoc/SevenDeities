#include "gfw_renderer.h"

namespace gfw
{
	void Renderer::SetSky(std::shared_ptr<SkyDome> sky)
	{
		m_sky = sky;
	}
	void Renderer::AddEntity(std::shared_ptr<Entity> entity)
	{
		m_entities.push_back(entity);
	}
	void Renderer::RemoveEntity(std::shared_ptr<Entity> entity)
	{
		for (auto e = m_entities.begin(); e != m_entities.end(); e++)
		{
			if (*e == entity)
			{
				m_entities.erase(e);
				return;
			}
		}
	}
	void Renderer::ClearEntities()
	{
		m_entities.clear();
	}
	void Renderer::RemoveSky()
	{
		m_sky.reset();
	}
	void Renderer::Clear()
	{
		ClearEntities();
		RemoveSky();
	}
}