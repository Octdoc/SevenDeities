#pragma once

#include "octdoc.h"

namespace cvt
{
	class CVTRenderer :public octdoc::gfx::Renderer
	{
		SHARED_ONLY(CVTRenderer)

		float m_ambient;
		octdoc::gfx::CBuffer::P m_psLightBuffer;
		octdoc::gfx::SamplerState::P m_sampler;

	private:
		CVTRenderer();
		CVTRenderer(octdoc::gfx::Graphics::P graphics);
		void CreateRenderer(octdoc::gfx::Graphics::P graphics);

	public:

		static CVTRenderer::P Create();
		static CVTRenderer::P Create(octdoc::gfx::Graphics::P graphics);

		virtual void Render(octdoc::gfx::Graphics::P graphics, octdoc::gfx::Camera& camera) override;
	};
}