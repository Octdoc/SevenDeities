#pragma once

#include "graphics/entity/gfx_entity.h"
#include "graphics/entity/gfx_skydome.h"
#include "graphics/entity/gfx_camera.h"

namespace octdoc
{
	namespace gfx
	{
		class Renderer
		{
			SHARED_ONLY(Renderer);

		protected:
			Renderer() = default;

			SkyDome::P m_sky;
			gfx::PixelShader::P m_pixelShader;
			gfx::VertexShader::P m_vertexShader;
			gfx::CBuffer::P m_vsMatrixBuffer;

			std::vector<Entity::P> m_entities;

		public:
			void SetSky(SkyDome::P sky);
			void AddEntity(Entity::P entity);
			void RemoveEntity(Entity::P entity);
			void ClearEntities();
			void RemoveSky();
			void Clear();

			virtual void Render(Graphics::P graphics, Camera& camera) = 0;
		};
	}
}