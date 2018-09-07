#pragma once

#include "gfw_renderer.h"

namespace gfw
{
	class SimpleRenderer :public Renderer
	{
		float m_ambient;
		std::shared_ptr<gfw::PixelShader> m_pixelShader;
		std::shared_ptr<gfw::VertexShader> m_vertexShader;
		std::shared_ptr<gfw::CBuffer> m_vsBuffer;
		std::shared_ptr<gfw::CBuffer> m_psBuffer;
		std::shared_ptr<gfw::SamplerState> m_sampler;
		
	public:
		SimpleRenderer();
		SimpleRenderer(Graphics& graphics);
		void CreateRenderer(Graphics& graphics);

		static std::shared_ptr<SimpleRenderer> Create();
		static std::shared_ptr<SimpleRenderer> Create(Graphics& graphics);

		virtual void Render(Graphics& graphics, Camera& camera) override;
	};
}