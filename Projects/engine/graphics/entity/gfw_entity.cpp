#include "gfw_entity.h"

namespace gfw
{
	void Entity::CreateEntity(Model::P model)
	{
		m_model = model;
	}
	void Entity::CreateEntity(Model::P model, Texture::P texture)
	{
		m_model = model;
		m_textures.push_back(texture);
	}
	void Entity::CreateEntity(Model::P model, Texture::P texture, Texture::P normalmap)
	{
		m_model = model;
		m_textures.push_back(texture);
		m_normalmaps.push_back(normalmap);
	}
	void Entity::CreateEntity(Model::P model, Texture::P textures[])
	{
		m_model = model;
		for (UINT i = 0; i < model->getIndexGroupCount(); i++)
			m_textures.push_back(textures[i]);
	}
	void Entity::CreateEntity(Model::P model, Texture::P textures[], Texture::P normalmaps[])
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
	Model::P Entity::getModel()
	{
		return m_model;
	}
	std::vector<Texture::P>& Entity::getTextures()
	{
		return m_textures;
	}
	std::vector<Texture::P>& Entity::getNormalmaps()
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
					m_textures[i]->SetTextureToRender(deviceContext, 0);
				if (m_normalmaps.size() > i)
					m_normalmaps[i]->SetTextureToRender(deviceContext, 1);
				m_model->RenderPart_DrawCallOnly(deviceContext, i);
			}
		}
	}
	void Entity::RenderModel(ID3D11DeviceContext* deviceContext)
	{
		if (m_shown)
			m_model->Render(deviceContext);
	}
	Entity::Entity(Model::P model) : m_shown(true)
	{
		CreateEntity(model);
	}
	Entity::Entity(Model::P model, Texture::P texture) : m_shown(true)
	{
		CreateEntity(model, texture);
	}
	Entity::Entity(Model::P model, Texture::P texture, Texture::P normalmap) : m_shown(true)
	{
		CreateEntity(model, texture, normalmap);
	}
	Entity::Entity(Model::P model, Texture::P textures[]) : m_shown(true)
	{
		CreateEntity(model, textures);
	}
	Entity::Entity(Model::P model, Texture::P textures[], Texture::P normalmaps[]) : m_shown(true)
	{
		CreateEntity(model, textures, normalmaps);
	}
	Entity::P Entity::Create(Model::P model)
	{
		return std::make_shared<Entity>(model);
	}
	Entity::P Entity::Create(Model::P model, Texture::P texture)
	{
		return std::make_shared<Entity>(model, texture);
	}
	Entity::P Entity::Create(Model::P model, Texture::P texture, Texture::P normalmap)
	{
		return std::make_shared<Entity>(model, texture, normalmap);
	}
	Entity::P Entity::Create(Model::P model, Texture::P textures[])
	{
		return std::make_shared<Entity>(model, textures);
	}
	Entity::P Entity::Create(Model::P model, Texture::P textures[], Texture::P normalmaps[])
	{
		return std::make_shared<Entity>(model, textures, normalmaps);
	}
}