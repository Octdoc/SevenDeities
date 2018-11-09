#include "gfx_simpleshadowrenderer.h"

namespace octdoc
{
	namespace gfx
	{
		void SimpleShadowRenderer::RenderScene(Graphics::P graphics, Camera& camera)
		{
			ID3D11DeviceContext* deviceContext = graphics->getDeviceContext();
			mth::float4x4 shadowMatrixBuffer[3];

			graphics->SetViewPort();
			graphics->RenderToScreenSetTarget();
			if (m_sky)
				m_sky->Render(deviceContext, camera);

			m_ssClamp->SetSamplerStateToRender(deviceContext, 0);
			m_ssWrap->SetSamplerStateToRender(deviceContext, 1);
			m_vertexShader->SetShaderToRender(deviceContext);
			m_pixelShader->SetShaderToRender(deviceContext);
			shadowMatrixBuffer[1] = camera.GetCameraMatrix();
			shadowMatrixBuffer[2] = m_light.GetLightMatrix();
			VertexShader::SetCBuffer(deviceContext, m_vsMatrixBuffer, 0);
			FillLightBuffers(deviceContext);
			m_shadowTexture->SetTextureToRender(deviceContext, 2);
			for (auto& e : m_entities)
			{
				shadowMatrixBuffer[0] = mth::float4x4::Identity();
				e->RenderWithSubparts(deviceContext, shadowMatrixBuffer, m_vsMatrixBuffer);
			}
			graphics->Present();
		}

		SimpleShadowRenderer::SimpleShadowRenderer(Graphics::P graphics, UINT shadowmapSize, bool cubicShadowMap)
		{
			CreateRenderer(graphics, shadowmapSize, cubicShadowMap);
		}
		void SimpleShadowRenderer::CreateRenderer(Graphics::P graphics, UINT shadowmapSize, bool cubicShadowMap)
		{
			ID3D11Device* device = graphics->getDevice();

			CreateDepthRenderingResources(device, shadowmapSize, cubicShadowMap);

			m_vertexShader = VertexShader::Create(device, L"Shaders/vsShadow.cso", SIL_POSITION | SIL_TEXCOORD | SIL_NORMAL | SIL_NORMALMAP);
			m_pixelShader = PixelShader::Create(device, m_cubicShadowMap ? L"Shaders/psShadowCube.cso" : L"Shaders/psShadow.cso");
			m_vsMatrixBuffer = CBuffer::Create(device, sizeof(mth::float4x4) * 3);
			m_vsLightBuffer = CBuffer::Create(device, sizeof(float) * 4);
			m_psLightBuffer = CBuffer::Create(device, sizeof(float) * 12);
			m_ssWrap = SamplerState::Create(device, true, D3D11_TEXTURE_ADDRESS_WRAP);
		}
		SimpleShadowRenderer::P SimpleShadowRenderer::Create(Graphics::P graphics, UINT shadowmapSize, bool cubicShadowMap)
		{
			return std::make_shared<SimpleShadowRenderer>(graphics, shadowmapSize, cubicShadowMap);
		}
	}
}