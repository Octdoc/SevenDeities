#pragma once

#include "gfx_renderer.h"

namespace octdoc
{
	namespace gfx
	{
		class ColorRenderer :public Renderer
		{
			SHARED_ONLY(ColorRenderer)

			float m_ambient;
			gfx::CBuffer::P m_psLightBuffer;
			gfx::CBuffer::P m_psColorBuffer;

		private:
			ColorRenderer();
			ColorRenderer(gfx::Graphics::P graphics);
			void CreateRenderer(gfx::Graphics::P graphics);

		public:

			static ColorRenderer::P Create();
			static ColorRenderer::P Create(gfx::Graphics::P graphics);

			virtual void Render(gfx::Graphics::P graphics, gfx::Camera& camera) override;
		};

	}
}