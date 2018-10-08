#include "gfw_colorshadowrenderer.h"

namespace gfw
{
	void ColorShadowRenderer::RenderScene(Graphics::P graphics, Camera& camera)
	{
		ID3D11DeviceContext* deviceContext = graphics->getDeviceContext();
		mth::float4x4 shadowMatrixBuffer[3];

		graphics->SetViewPort();
		graphics->RenderToScreenSetTarget();
		if (m_sky)
			m_sky->Render(deviceContext, camera);

		m_ssClamp->SetSamplerStateToRender(deviceContext);
		m_vertexShader->SetShaderToRender(deviceContext);
		m_pixelShader->SetShaderToRender(deviceContext);
		shadowMatrixBuffer[1] = camera.GetCameraMatrix();
		shadowMatrixBuffer[2] = m_light.GetLightMatrix();
		VertexShader::SetCBuffer(deviceContext, m_vsMatrixBuffer, 0);
		FillLightBuffers(deviceContext);
		m_shadowTexture->SetTextureToRender(deviceContext, 0);
		gfw::PixelShader::SetCBuffer(deviceContext, m_psColorBuffer, 1);
		for (auto& e : m_entities)
		{
			shadowMatrixBuffer[0] = mth::float4x4::Identity();
			e->RenderWithSubparts(deviceContext, shadowMatrixBuffer, m_vsMatrixBuffer, m_psColorBuffer);
		}
		graphics->Present();
	}

	ColorShadowRenderer::ColorShadowRenderer(Graphics::P graphics, UINT shadowmapSize, bool cubicShadowMap)
	{
		CreateRenderer(graphics, shadowmapSize, cubicShadowMap);
	}
	void ColorShadowRenderer::CreateRenderer(Graphics::P graphics, UINT shadowmapSize, bool cubicShadowMap)
	{
		ID3D11Device* device = graphics->getDevice();

		CreateDepthRenderingResources(device, shadowmapSize, cubicShadowMap);

		m_vertexShader = VertexShader::Create(device, L"Shaders/vsColorShadow.cso", SIL_POSITION | SIL_NORMAL);
		m_pixelShader = PixelShader::Create(device, m_cubicShadowMap ? L"Shaders/psColorShadowCube.cso" : L"Shaders/psColorShadow.cso");
		m_vsMatrixBuffer = CBuffer::Create(device, sizeof(mth::float4x4) * 3);
		m_vsLightBuffer = CBuffer::Create(device, sizeof(float) * 4);
		m_psLightBuffer = CBuffer::Create(device, sizeof(float) * 12);
		m_psColorBuffer = gfw::CBuffer::Create(device, sizeof(float) * 4);
	}
	ColorShadowRenderer::P ColorShadowRenderer::Create(Graphics::P graphics, UINT shadowmapSize, bool cubicShadowMap)
	{
		return std::make_shared<ColorShadowRenderer>(graphics, shadowmapSize, cubicShadowMap);
	}
}