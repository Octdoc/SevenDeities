#pragma once

#include "gfx_shadowrenderer.h"

namespace octdoc
{
	namespace gfx
	{
		class ColorShadowRenderer :public ShadowRenderer
		{
			SHARED_ONLY(ColorShadowRenderer);

			gfx::CBuffer::P m_psColorBuffer;

		private:
			ColorShadowRenderer(Graphics::P graphics, UINT shadowmapSize, bool cubicShadowMap);
			void CreateRenderer(Graphics::P graphics, UINT shadowmapSize, bool cubicShadowMap);

			void RenderScene(Graphics::P graphics, Camera& camera);

		public:
			static ColorShadowRenderer::P Create(Graphics::P graphics, UINT shadowmapSize = 1024, bool cubicShadowMap = false);
		};
	}
}