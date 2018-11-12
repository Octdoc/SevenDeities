#pragma once

#include "graphics/resources/gfx_model.h"
#include "graphics/resources/gfx_shader.h"
#include "graphics/resources/gfx_texture.h"
#include "math/mth_position.h"

namespace octdoc
{
	namespace gfx
	{

		class Entity :public mth::Position
		{
			SHARED_ONLY(Entity)

			mth::float4 m_color;
			std::vector<Model::P> m_models;
			std::vector<Texture::P> m_textures;
			std::vector<Texture::P> m_normalmaps;
			bool m_shown;
			std::vector<Entity::P> m_subparts;
			Entity::P m_relativeTo;

		private:
			Entity(Model::P *models, Texture::P *textures, Texture::P *normalmaps, UINT count);
			void CreateEntity(Model::P *models, Texture::P *textures, Texture::P *normalmaps, UINT count);

		public:
			static Entity::P Create(Model::P model);
			static Entity::P Create(Model::P model, Texture::P texture);
			static Entity::P Create(Model::P model, Texture::P texture, Texture::P normalmap);
			static Entity::P Create(Model::P *models, UINT count);
			static Entity::P Create(Model::P *models, Texture::P *textures, UINT count);
			static Entity::P Create(Model::P *models, Texture::P *textures, Texture::P *normalmaps, UINT count);
			void Clear();

			void Hide();
			void Show();

			void Render(ID3D11DeviceContext* deviceContext);
			void RenderWithSubparts(ID3D11DeviceContext* deviceContext, mth::float4x4 *matrixBuffer,
				CBuffer::P vsMatrixBuffer, CBuffer::P psColorBuffer = nullptr);
			void RenderModel(ID3D11DeviceContext* deviceContext);

			void setColor(mth::float4 color);
			mth::float4 getColor();
			Model::P getModel(UINT index);
			Texture::P getTexture(UINT index);
			Texture::P getNormalmap(UINT index);
			std::vector<Model::P>& getModels();
			std::vector<Texture::P>& getTextures();
			std::vector<Texture::P>& getNormalmaps();
			void AddSubpart(Entity::P entity);
			Entity::P getSubpart(UINT index);
			UINT getSubpartCount();
			Entity::P getRelativeTo();
			void setRelativeTo(Entity::P entity);
		};
	}
}