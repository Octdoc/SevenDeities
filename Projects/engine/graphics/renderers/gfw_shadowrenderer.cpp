#include "gfw_shadowrenderer.h"

namespace gfw
{
	void ShadowRenderer::RenderShadowMap(Graphics::P graphics, Camera& camera)
	{
		ID3D11DeviceContext* deviceContext = graphics->getDeviceContext();
		mth::float4x4 depthMatrixBuffer[2];
		UINT shadowMapFaces = m_cubicShadowMap ? 6 : 1;

		graphics->SetViewPort((float)m_shadowTexture->getWidth(), (float)m_shadowTexture->getHeight());
		m_vsDepth->SetShaderToRender(deviceContext);
		m_psDepth->SetShaderToRender(deviceContext);
		VertexShader::SetCBuffer(deviceContext, m_depthMatrixBuffer);
		for (UINT i = 0; i < shadowMapFaces; i++)
		{
			graphics->RenderToSurface(m_shadowTexture->getRenderTargetView(i), m_shadowTexture->getDepthStencilView());
			depthMatrixBuffer[1] = m_light.GetLightMatrix(i);
			for (auto& e : m_entities)
			{
				depthMatrixBuffer[0] = mth::float4x4::Identity();
				e->RenderWithSubparts(deviceContext, depthMatrixBuffer, m_depthMatrixBuffer);
			}
		}
	}
	void ShadowRenderer::FillLightBuffers(ID3D11DeviceContext *deviceContext)
	{
		float vsLightBuffer[4];
		float psLightBuffer[12];

		vsLightBuffer[0] = m_light.position.x;
		vsLightBuffer[1] = m_light.position.y;
		vsLightBuffer[2] = m_light.position.z;
		vsLightBuffer[3] = 0.0f;
		m_vsLightBuffer->WriteBuffer(deviceContext, vsLightBuffer);
		VertexShader::SetCBuffer(deviceContext, m_vsLightBuffer, 1);
		psLightBuffer[0] = 0.25f;
		psLightBuffer[1] = 0.25f;
		psLightBuffer[2] = 0.25f;
		psLightBuffer[3] = 1.0f;
		psLightBuffer[4] = 1.0f;
		psLightBuffer[5] = 1.0f;
		psLightBuffer[6] = 1.0f;
		psLightBuffer[7] = 1.0f;
		psLightBuffer[8] = m_light.position.x;
		psLightBuffer[9] = m_light.position.y;
		psLightBuffer[10] = m_light.position.z;
		psLightBuffer[11] = 0.5f;
		m_psLightBuffer->WriteBuffer(deviceContext, psLightBuffer);
		PixelShader::SetCBuffer(deviceContext, m_psLightBuffer);
	}
	void ShadowRenderer::CreateDepthRenderingResources(ID3D11Device *device, UINT shadowmapSize, bool cubicShadowMap)
	{
		m_cubicShadowMap = cubicShadowMap;
		m_vsDepth = VertexShader::Create(device, L"Shaders/vsDepth.cso", SIL_POSITION);
		m_psDepth = PixelShader::Create(device, L"Shaders/psDepth.cso");
		m_shadowTexture = RenderTarget::Create(device, shadowmapSize, shadowmapSize, m_cubicShadowMap);
		m_depthMatrixBuffer = CBuffer::Create(device, sizeof(mth::float4x4) * 2);
		m_ssClamp = SamplerState::Create(device, true, D3D11_TEXTURE_ADDRESS_CLAMP);

		if (m_cubicShadowMap)
		{
			m_light.InitCube();
		}
		else
		{
			m_light.Init(true, mth::pi / 2.0f);
			m_light.rotation = { 0.5f, 0.36f, 0.0f };
		}
		m_light.position = { -3.0f, 6.4f, -8.0f };
	}
	Light& ShadowRenderer::getLight()
	{
		return m_light;
	}

	void ShadowRenderer::Render(Graphics::P graphics, Camera& camera)
	{
		m_light.Update();
		camera.Update();
		RenderShadowMap(graphics, camera);
		RenderScene(graphics, camera);
	}

}
