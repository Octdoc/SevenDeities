#include "gfw_entity.h"

namespace gfw
{
	void Entity::CreateEntity(std::shared_ptr<Model> model)
	{
		m_model = model;
	}
	void Entity::CreateEntity(std::shared_ptr<Model> model, std::shared_ptr<Texture> texture)
	{
		m_model = model;
		m_textures.push_back(texture);
	}
	void Entity::CreateEntity(std::shared_ptr<Model> model, std::shared_ptr<Texture> texture, std::shared_ptr<Texture> normalmap)
	{
		m_model = model;
		m_textures.push_back(texture);
		m_normalmaps.push_back(normalmap);
	}
	void Entity::CreateEntity(std::shared_ptr<Model> model, std::shared_ptr<Texture> textures[])
	{
		m_model = model;
		for (UINT i = 0; i < model->getIndexGroupCount(); i++)
			m_textures.push_back(textures[i]);
	}
	void Entity::CreateEntity(std::shared_ptr<Model> model, std::shared_ptr<Texture> textures[], std::shared_ptr<Texture> normalmaps[])
	{
		m_model = model;
		for (UINT i = 0; i < model->getIndexGroupCount(); i++)
		{
			m_textures.push_back(textures[i]);
			m_normalmaps.push_back(normalmaps[i]);
		}
	}
	void Entity::Clear()
	{
		m_model.reset();
		m_textures.clear();
		m_normalmaps.clear();
	}
	void Entity::Hide()
	{
		m_shown = false;
	}
	void Entity::Show()
	{
		m_shown = true;
	}
	std::weak_ptr<Model> Entity::getModel()
	{
		return m_model;
	}
	std::vector<std::shared_ptr<Texture>>& Entity::getTextures()
	{
		return m_textures;
	}
	std::vector<std::shared_ptr<Texture>>& Entity::getNormalmaps()
	{
		return m_normalmaps;
	}
	void Entity::Render(ID3D11DeviceContext* deviceContext)
	{
		if (m_shown)
		{
			m_model->SetBuffersToRender(deviceContext);
			for (UINT i = 0; i < m_model->getIndexGroupCount(); i++)
			{
				if (m_textures.size() > i)
					m_textures[i]->SetTexture(deviceContext, 0);
				if (m_normalmaps.size() > i)
					m_normalmaps[i]->SetTexture(deviceContext, 1);
				m_model->RenderPart_DrawCallOnly(deviceContext, i);
			}
		}
	}
	void Entity::RenderModel(ID3D11DeviceContext* deviceContext)
	{
		if (m_shown)
			m_model->Render(deviceContext);
	}
	Entity::Entity() : m_shown(true) {}
	Entity::Entity(std::shared_ptr<Model> model) : m_shown(true)
	{
		CreateEntity(model);
	}
	Entity::Entity(std::shared_ptr<Model> model, std::shared_ptr<Texture> texture) : m_shown(true)
	{
		CreateEntity(model, texture);
	}
	Entity::Entity(std::shared_ptr<Model> model, std::shared_ptr<Texture> texture, std::shared_ptr<Texture> normalmap) : m_shown(true)
	{
		CreateEntity(model, texture, normalmap);
	}
	Entity::Entity(std::shared_ptr<Model> model, std::shared_ptr<Texture> textures[]) : m_shown(true)
	{
		CreateEntity(model, textures);
	}
	Entity::Entity(std::shared_ptr<Model> model, std::shared_ptr<Texture> textures[], std::shared_ptr<Texture> normalmaps[]) : m_shown(true)
	{
		CreateEntity(model, textures, normalmaps);
	}
	std::shared_ptr<Entity> Entity::Create()
	{
		return std::make_shared<Entity>();
	}
	std::shared_ptr<Entity> Entity::Create(std::shared_ptr<Model> model)
	{
		return std::make_shared<Entity>(model);
	}
	std::shared_ptr<Entity> Entity::Create(std::shared_ptr<Model> model, std::shared_ptr<Texture> texture)
	{
		return std::make_shared<Entity>(model, texture);
	}
	std::shared_ptr<Entity> Entity::Create(std::shared_ptr<Model> model, std::shared_ptr<Texture> texture, std::shared_ptr<Texture> normalmap)
	{
		return std::make_shared<Entity>(model, texture, normalmap);
	}
	std::shared_ptr<Entity> Entity::Create(std::shared_ptr<Model> model, std::shared_ptr<Texture> textures[])
	{
		return std::make_shared<Entity>(model, textures);
	}
	std::shared_ptr<Entity> Entity::Create(std::shared_ptr<Model> model, std::shared_ptr<Texture> textures[], std::shared_ptr<Texture> normalmaps[])
	{
		return std::make_shared<Entity>(model, textures, normalmaps);
	}
}