#pragma once

#include "graphics/resources/gfw_model.h"
#include "graphics/resources/gfw_shader.h"
#include "graphics/resources/gfw_texture.h"
#include "helpers/mth_position.h"

namespace gfw
{

	class Entity :public mth::Position
	{
		SHARED_ONLY(Entity);
		Model::P m_model;
		std::vector<Texture::P> m_textures;
		std::vector<Texture::P> m_normalmaps;
		bool m_shown;

	private:
		Entity(Model::P model);
		Entity(Model::P model, Texture::P texture);
		Entity(Model::P model, Texture::P texture, Texture::P normalmap);
		Entity(Model::P model, Texture::P textures[]);
		Entity(Model::P model, Texture::P textures[], Texture::P normalmaps[]);
		void CreateEntity(Model::P model);
		void CreateEntity(Model::P model, Texture::P texture);
		void CreateEntity(Model::P model, Texture::P texture, Texture::P normalmap);
		void CreateEntity(Model::P model, Texture::P texture[]);
		void CreateEntity(Model::P model, Texture::P texture[], Texture::P normalmap[]);

	public:
		static Entity::P Create(Model::P model);
		static Entity::P Create(Model::P model, Texture::P texture);
		static Entity::P Create(Model::P model, Texture::P texture, Texture::P normalmap);
		static Entity::P Create(Model::P model, Texture::P textures[]);
		static Entity::P Create(Model::P model, Texture::P textures[], Texture::P normalmaps[]);
		void Clear();

		void Hide();
		void Show();

		void Render(ID3D11DeviceContext* deviceContext);
		void RenderModel(ID3D11DeviceContext* deviceContext);

		Model::P getModel();
		std::vector<Texture::P>& getTextures();
		std::vector<Texture::P>& getNormalmaps();
	};
}