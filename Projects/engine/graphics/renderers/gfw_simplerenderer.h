#pragma once

#include "gfw_renderer.h"

namespace gfw
{
	class SimpleRenderer :public Renderer
	{
		SHARED_ONLY(SimpleRenderer);

		float m_ambient;
		gfw::PixelShader::P m_pixelShader;
		gfw::VertexShader::P m_vertexShader;
		gfw::CBuffer::P m_vsBuffer;
		gfw::CBuffer::P m_psBuffer;
		gfw::SamplerState::P m_sampler;
		
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