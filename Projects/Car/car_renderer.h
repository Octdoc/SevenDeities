#pragma once

#include "octdoc.h"

namespace car
{
	class Renderer :public gfw::Renderer
	{
		SHARED_ONLY(Renderer);

		float m_ambient;
		gfw::PixelShader::P m_pixelShader;
		gfw::VertexShader::P m_vertexShader;
		gfw::CBuffer::P m_vsMatrixBuffer;
		gfw::CBuffer::P m_psLightBuffer;
		gfw::CBuffer::P m_psColorBuffer;

	private:
		Renderer();
		Renderer(gfw::Graphics::P graphics);
		void CreateRenderer(gfw::Graphics::P graphics);

	public:

		static Renderer::P Create();
		static Renderer::P Create(gfw::Graphics::P graphics);

		virtual void Render(gfw::Graphics::P graphics, gfw::Camera& camera) override;
	};

}