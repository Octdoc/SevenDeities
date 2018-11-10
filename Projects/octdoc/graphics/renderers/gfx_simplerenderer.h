#pragma once

#include "gfx_renderer.h"

namespace octdoc
{
	namespace gfx
	{
		class SimpleRenderer :public Renderer
		{
			SHARED_ONLY(SimpleRenderer)

			float m_ambient;
			gfx::CBuffer::P m_psLightBuffer;
			gfx::SamplerState::P m_sampler;

		private:
			SimpleRenderer();
			SimpleRenderer(Graphics::P graphics);
			void CreateRenderer(Graphics::P graphics);

		public:

			static SimpleRenderer::P Create();
			static SimpleRenderer::P Create(Graphics::P graphics);

			virtual void Render(Graphics::P graphics, Camera& camera) override;
		};
	}
}