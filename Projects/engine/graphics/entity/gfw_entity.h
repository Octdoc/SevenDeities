#pragma once

#include "graphics/resources/gfw_model.h"
#include "graphics/resources/gfw_shader.h"
#include "graphics/resources/gfw_texture.h"
#include "helpers/mth_position.h"

namespace gfw
{

	class Entity :public mth::Position
	{
		std::shared_ptr<Model> m_model;
		std::vector<std::shared_ptr<Texture>> m_textures;
		std::vector<std::shared_ptr<Texture>> m_normalmaps;
		bool m_shown;

	public:
		Entity();
		Entity(std::shared_ptr<Model> model);
		Entity(std::shared_ptr<Model> model, std::shared_ptr<Texture> texture);
		Entity(std::shared_ptr<Model> model, std::shared_ptr<Texture> texture, std::shared_ptr<Texture> normalmap);
		Entity(std::shared_ptr<Model> model, std::shared_ptr<Texture> textures[]);
		Entity(std::shared_ptr<Model> model, std::shared_ptr<Texture> textures[], std::shared_ptr<Texture> normalmaps[]);
		static std::shared_ptr<Entity> Create();
		static std::shared_ptr<Entity> Create(std::shared_ptr<Model> model);
		static std::shared_ptr<Entity> Create(std::shared_ptr<Model> model, std::shared_ptr<Texture> texture);
		static std::shared_ptr<Entity> Create(std::shared_ptr<Model> model, std::shared_ptr<Texture> texture, std::shared_ptr<Texture> normalmap);
		static std::shared_ptr<Entity> Create(std::shared_ptr<Model> model, std::shared_ptr<Texture> textures[]);
		static std::shared_ptr<Entity> Create(std::shared_ptr<Model> model, std::shared_ptr<Texture> textures[], std::shared_ptr<Texture> normalmaps[]);
		void CreateEntity(std::shared_ptr<Model> model);
		void CreateEntity(std::shared_ptr<Model> model, std::shared_ptr<Texture> texture);
		void CreateEntity(std::shared_ptr<Model> model, std::shared_ptr<Texture> texture, std::shared_ptr<Texture> normalmap);
		void CreateEntity(std::shared_ptr<Model> model, std::shared_ptr<Texture> texture[]);
		void CreateEntity(std::shared_ptr<Model> model, std::shared_ptr<Texture> texture[], std::shared_ptr<Texture> normalmap[]);
		void Clear();

		void Hide();
		void Show();

		void Render(ID3D11DeviceContext* deviceContext);
		void RenderModel(ID3D11DeviceContext* deviceContext);

		std::weak_ptr<Model> getModel();
		std::vector<std::shared_ptr<Texture>>& getTextures();
		std::vector<std::shared_ptr<Texture>>& getNormalmaps();
	};
}