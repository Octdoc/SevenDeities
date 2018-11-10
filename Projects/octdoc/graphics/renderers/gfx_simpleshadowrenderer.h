#pragma once

#include "gfx_shadowrenderer.h"

namespace octdoc
{
	namespace gfx
	{
		class SimpleShadowRenderer :public ShadowRenderer
		{
			SHARED_ONLY(SimpleShadowRenderer)

			SamplerState::P m_ssWrap;

		private:
			SimpleShadowRenderer(Graphics::P graphics, UINT shadowmapSize, bool cubicShadowMap);
			void CreateRenderer(Graphics::P graphics, UINT shadowmapSize, bool cubicShadowMap);

			virtual void RenderScene(Graphics::P graphics, Camera& camera) override;

		public:
			static SimpleShadowRenderer::P Create(Graphics::P graphics, UINT shadowmapSize = 1024, bool cubicShadowMap = false);
		};
	}
}