#include "gfw_renderer.h"

namespace gfw
{
	void Renderer::SetSky(SkyDome::P sky)
	{
		m_sky = sky;
	}
	void Renderer::AddEntity(Entity::P entity)
	{
		m_entities.push_back(entity);
	}
	void Renderer::RemoveEntity(Entity::P entity)
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