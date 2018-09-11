#pragma once

#include "gfw_renderer.h"
#include "graphics/entity/gfw_light.h"

namespace gfw
{
	class ShadowRenderer :public Renderer
	{
		SHARED_ONLY(ShadowRenderer);

		VertexShader::P m_vsDepth;
		PixelShader::P m_psDepth;
		RenderTarget::P m_shadowTexture;
		CBuffer::P m_depthMatrixBuffer;

		VertexShader::P m_vsShadow;
		PixelShader::P m_psShadow;
		CBuffer::P m_shadowMatrixBuffer;
		CBuffer::P m_vsLightBuffer;
		CBuffer::P m_psLightBuffer;
		SamplerState::P m_ssWrap;
		SamplerState::P m_ssClamp;

		bool m_cubicShadowMap;

		Light m_light;

	private:
		ShadowRenderer();
		ShadowRenderer(Graphics::P graphics);
		ShadowRenderer(Graphics::P graphics, UINT shadowmapSize, bool cubicShadowMap);
		void CreateRenderer(Graphics::P graphics);
		void CreateRenderer(Graphics::P graphics, UINT shadowmapSize, bool cubicShadowMap);

		void RenderShadowMap(Graphics::P graphics, Camera& camera);
		void RenderScene(Graphics::P graphics, Camera& camera);

	public:

		static ShadowRenderer::P Create();
		static ShadowRenderer::P Create(Graphics::P graphics);
		static ShadowRenderer::P Create(Graphics::P graphics, UINT shadowmapSize, bool cubicShadowMap);

		virtual void Render(Graphics::P graphics, Camera& camera);
	};
}