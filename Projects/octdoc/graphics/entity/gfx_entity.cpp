#include "gfx_entity.h"

namespace octdoc
{
	namespace gfx
	{
		void Entity::CreateEntity(Model::P *models, Texture::P *textures, Texture::P *normalmaps, UINT count)
		{
			for (UINT i = 0; i < count; i++)
			{
				m_models.push_back(models[i]);
				m_textures.push_back(textures ? textures[i] : nullptr);
				m_normalmaps.push_back(normalmaps ? normalmaps[i] : nullptr);
			}
		}
		void Entity::Clear()
		{
			m_models.clear();
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
		Model::P Entity::getModel(UINT index)
		{
			return m_models[index];
		}
		Texture::P Entity::getTexture(UINT index)
		{
			return m_textures[index];
		}
		Texture::P Entity::getNormalmap(UINT index)
		{
			return m_normalmaps[index];
		}
		std::vector<Model::P>& Entity::getModels()
		{
			return m_models;
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
				for (UINT i = 0; i < m_models.size(); i++)
				{
					if (m_textures[i])
						m_textures[i]->SetTextureToRender(deviceContext, 0);
					if (m_normalmaps[i])
						m_normalmaps[i]->SetTextureToRender(deviceContext, 1);
					m_models[i]->Render(deviceContext);
				}
			}
		}
		void Entity::RenderModel(ID3D11DeviceContext* deviceContext)
		{
			if (m_shown)
				for (auto& m : m_models)
					m->Render(deviceContext);
		}
		Entity::Entity(Model::P *models, Texture::P *textures, Texture::P *normalmaps, UINT count) : m_shown(true)
		{
			CreateEntity(models, textures, normalmaps, count);
		}
		Entity::P Entity::Create(Model::P model)
		{
			return std::make_shared<Entity>(&model, nullptr, nullptr, 1);
		}
		Entity::P Entity::Create(Model::P model, Texture::P texture)
		{
			return std::make_shared<Entity>(&model, &texture, nullptr, 1);
		}
		Entity::P Entity::Create(Model::P model, Texture::P texture, Texture::P normalmap)
		{
			return std::make_shared<Entity>(&model, &texture, &normalmap, 1);
		}
		Entity::P Entity::Create(Model::P *models, UINT count)
		{
			return std::make_shared<Entity>(models, nullptr, nullptr, count);
		}
		Entity::P Entity::Create(Model::P *models, Texture::P *textures, UINT count)
		{
			return std::make_shared<Entity>(models, textures, nullptr, count);
		}
		Entity::P Entity::Create(Model::P *models, Texture::P *textures, Texture::P *normalmaps, UINT count)
		{
			return std::make_shared<Entity>(models, textures, normalmaps, count);
		}
	}
}