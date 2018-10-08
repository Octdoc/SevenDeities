#pragma once

#include "gfw_renderer.h"
#include "graphics/entity/gfw_light.h"

namespace gfw
{
	class ShadowRenderer :public Renderer
	{
		SHARED_ONLY(ShadowRenderer);

	protected:
		ShadowRenderer() = default;

		VertexShader::P m_vsDepth;
		PixelShader::P m_psDepth;
		RenderTarget::P m_shadowTexture;
		CBuffer::P m_depthMatrixBuffer;

		CBuffer::P m_vsLightBuffer;
		CBuffer::P m_psLightBuffer;
		SamplerState::P m_ssClamp;

		bool m_cubicShadowMap;

		Light m_light;

	protected:
		void RenderShadowMap(Graphics::P graphics, Camera& camera);
		void FillLightBuffers(ID3D11DeviceContext *deviceContext);
		void CreateDepthRenderingResources(ID3D11Device *device, UINT shadowmapSize, bool cubicShadowMap);

		virtual void RenderScene(Graphics::P graphics, Camera& camera) = 0;

	public:
		Light& getLight();

		virtual void Render(Graphics::P graphics, Camera& camera) override;
	};
}