#pragma once

#include "gfw_renderer.h"

namespace gfw
{
	class ColorRenderer :public Renderer
	{
		SHARED_ONLY(ColorRenderer);

		float m_ambient;
		gfw::CBuffer::P m_psLightBuffer;
		gfw::CBuffer::P m_psColorBuffer;

	private:
		ColorRenderer();
		ColorRenderer(gfw::Graphics::P graphics);
		void CreateRenderer(gfw::Graphics::P graphics);

	public:

		static ColorRenderer::P Create();
		static ColorRenderer::P Create(gfw::Graphics::P graphics);

		virtual void Render(gfw::Graphics::P graphics, gfw::Camera& camera) override;
	};

}