#pragma once

#include "gfw_renderer.h"
#include "graphics/entity/gfw_light.h"

namespace gfw
{
	class ShadowRenderer :public Renderer
	{
		std::shared_ptr<VertexShader> m_vsDepth;
		std::shared_ptr<PixelShader> m_psDepth;
		std::shared_ptr<RenderTarget> m_shadowTexture;
		std::shared_ptr<CBuffer> m_depthMatrixBuffer;

		std::shared_ptr<VertexShader> m_vsShadow;
		std::shared_ptr<PixelShader> m_psShadow;
		std::shared_ptr<CBuffer> m_shadowMatrixBuffer;
		std::shared_ptr<CBuffer> m_vsLightBuffer;
		std::shared_ptr<CBuffer> m_psLightBuffer;
		std::shared_ptr<SamplerState> m_ssWrap;
		std::shared_ptr<SamplerState> m_ssClamp;

		bool m_cubicShadowMap;

		Light m_light;

	private:
		void RenderShadowMap(Graphics& graphics, Camera& camera);
		void RenderScene(Graphics& graphics, Camera& camera);

	public:
		ShadowRenderer();
		ShadowRenderer(Graphics& graphics);
		ShadowRenderer(Graphics& graphics, UINT shadowmapSize, bool cubicShadowMap);
		void CreateRenderer(Graphics& graphics);
		void CreateRenderer(Graphics& graphics, UINT shadowmapSize, bool cubicShadowMap);

		static std::shared_ptr<ShadowRenderer> Create();
		static std::shared_ptr<ShadowRenderer> Create(Graphics& graphics);
		static std::shared_ptr<ShadowRenderer> Create(Graphics& graphics, UINT shadowmapSize, bool cubicShadowMap);

		virtual void Render(Graphics& graphics, Camera& camera);
	};
}