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
	void Entity::setColor(mth::float4 color)
	{
		m_color = color;
	}
	mth::float4 Entity::getColor()
	{
		return m_color;
	}
	std::vector<Texture::P>& Entity::getTextures()
	{
		return m_textures;
	}
	std::vector<Texture::P>& Entity::getNormalmaps()
	{
		return m_normalmaps;
	}
	void Entity::AddSubpart(Entity::P entity)
	{
		m_subparts.push_back(entity);
	}
	Entity::P Entity::getSubpart(UINT index)
	{
		return m_subparts[index];
	}
	UINT Entity::getSubpartCount()
	{
		return (UINT)m_subparts.size();
	}
	Entity::P Entity::getRelativeTo()
	{
		return m_relativeTo;
	}
	void Entity::setRelativeTo(Entity::P entity)
	{
		m_relativeTo = entity;
	}
	void Entity::RenderWithSubparts(ID3D11DeviceContext *deviceContext, mth::float4x4 *matrixBuffer, CBuffer::P vsMatrixBuffer, CBuffer::P psColorBuffer)
	{
		if (m_shown)
		{
			mth::float4x4 matrix = matrixBuffer[0] * GetWorldMatrix();
			matrixBuffer[0] = matrix;
			vsMatrixBuffer->WriteBuffer(deviceContext, matrixBuffer);
			if (psColorBuffer)
				psColorBuffer->WriteBuffer(deviceContext, &m_color);
			Render(deviceContext);
			for (auto e : m_subparts)
			{
				matrixBuffer[0] = matrix;
				e->RenderWithSubparts(deviceContext, matrixBuffer, vsMatrixBuffer, psColorBuffer);
			}
		}
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