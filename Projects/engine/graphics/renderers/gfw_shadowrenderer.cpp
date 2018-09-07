#include "gfw_shadowrenderer.h"


namespace gfw
{
	void ShadowRenderer::RenderShadowMap(Graphics& graphics, Camera& camera)
	{
		ID3D11DeviceContext* deviceContext = graphics.getDeviceContext();
		mth::float4x4 depthMatrixBuffer[2];
		UINT shadowMapFaces = m_cubicShadowMap ? 6 : 1;

		graphics.SetViewPort((float)m_shadowTexture->getWidth(), (float)m_shadowTexture->getHeight());
		m_vsDepth->SetShaderToRender(deviceContext);
		m_psDepth->SetShaderToRender(deviceContext);
		VertexShader::SetCBuffer(deviceContext, *m_depthMatrixBuffer);
		for (UINT i = 0; i < shadowMapFaces; i++)
		{
			graphics.RenderToSurface(m_shadowTexture->getRenderTargetView(i), m_shadowTexture->getDepthStencilView());
			depthMatrixBuffer[1] = m_light.GetLightMatrix(i);
			for (auto& e : m_entities)
			{
				depthMatrixBuffer[0] = e->GetWorldMatrix();
				m_depthMatrixBuffer->WriteBuffer(deviceContext, depthMatrixBuffer);
				e->RenderModel(deviceContext);
			}
		}
	}
	void ShadowRenderer::RenderScene(Graphics& graphics, Camera& camera)
	{
		ID3D11DeviceContext* deviceContext = graphics.getDeviceContext();
		mth::float4x4 shadowMatrixBuffer[3];
		float vsLightBuffer[4];
		float psLightBuffer[12];

		graphics.SetViewPort();
		graphics.RenderToScreenSetTarget();
		if (m_sky)
			m_sky->Render(deviceContext, camera);

		m_ssClamp->SetSamplerState(deviceContext, 0);
		m_ssWrap->SetSamplerState(deviceContext, 1);
		m_vsShadow->SetShaderToRender(deviceContext);
		m_psShadow->SetShaderToRender(deviceContext);
		shadowMatrixBuffer[1] = camera.GetCameraMatrix();
		shadowMatrixBuffer[2] = m_light.GetLightMatrix();
		VertexShader::SetCBuffer(deviceContext, *m_shadowMatrixBuffer, 0);
		vsLightBuffer[0] = m_light.position.x;
		vsLightBuffer[1] = m_light.position.y;
		vsLightBuffer[2] = m_light.position.z;
		vsLightBuffer[3] = 0.0f;
		m_vsLightBuffer->WriteBuffer(deviceContext, vsLightBuffer);
		VertexShader::SetCBuffer(deviceContext, *m_vsLightBuffer, 1);
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
		PixelShader::SetCBuffer(deviceContext, *m_psLightBuffer);
		m_shadowTexture->SetTexture(deviceContext, 2);
		for (auto& e : m_entities)
		{
			shadowMatrixBuffer[0] = e->GetWorldMatrix();
			m_shadowMatrixBuffer->WriteBuffer(deviceContext, shadowMatrixBuffer);
			e->Render(deviceContext);
		}
		graphics.Present();
	}

	ShadowRenderer::ShadowRenderer() : m_cubicShadowMap(false) {}
	ShadowRenderer::ShadowRenderer(Graphics& graphics) : m_cubicShadowMap(false)
	{
		CreateRenderer(graphics);
	}
	ShadowRenderer::ShadowRenderer(Graphics & graphics, UINT shadowmapSize, bool cubicShadowMap)
	{
		CreateRenderer(graphics, shadowmapSize, cubicShadowMap);
	}
	void ShadowRenderer::CreateRenderer(Graphics& graphics)
	{
		CreateRenderer(graphics, 1024, false);
	}
	void ShadowRenderer::CreateRenderer(Graphics& graphics, UINT shadowmapSize, bool cubicShadowMap)
	{
		ID3D11Device* device = graphics.getDevice();
		ID3D11DeviceContext* deviceContext = graphics.getDeviceContext();

		m_cubicShadowMap = cubicShadowMap;
		m_vsDepth = VertexShader::Create(device, L"Shaders/vsDepth.cso", SIL_POSITION | SIL_TEXCOORD | SIL_NORMAL | SIL_NORMALMAP);
		m_psDepth = PixelShader::Create(device, L"Shaders/psDepth.cso");
		m_shadowTexture = RenderTarget::Create(device, shadowmapSize, shadowmapSize, m_cubicShadowMap);
		m_depthMatrixBuffer = CBuffer::Create(device, sizeof(mth::float4x4) * 2);

		m_vsShadow = VertexShader::Create(device, L"Shaders/vsShadow.cso", SIL_POSITION | SIL_TEXCOORD | SIL_NORMAL | SIL_NORMALMAP);
		m_psShadow = PixelShader::Create(device, m_cubicShadowMap ? L"Shaders/psShadowCube.cso" : L"Shaders/psShadow.cso");
		m_shadowMatrixBuffer = CBuffer::Create(device, sizeof(mth::float4x4) * 3);
		m_vsLightBuffer = CBuffer::Create(device, sizeof(float) * 4);
		m_psLightBuffer = CBuffer::Create(device, sizeof(float) * 12);
		m_ssWrap = SamplerState::Create(device, true, D3D11_TEXTURE_ADDRESS_WRAP);
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
	std::shared_ptr<ShadowRenderer> ShadowRenderer::Create()
	{
		return std::make_shared<ShadowRenderer>();
	}
	std::shared_ptr<ShadowRenderer> ShadowRenderer::Create(Graphics& graphics)
	{
		return std::make_shared<ShadowRenderer>(graphics);
	}
	std::shared_ptr<ShadowRenderer> ShadowRenderer::Create(Graphics& graphics, UINT shadowmapSize, bool cubicShadowMap)
	{
		return std::make_shared<ShadowRenderer>(graphics, shadowmapSize, cubicShadowMap);
	}
	void ShadowRenderer::Render(Graphics& graphics, Camera& camera)
	{
		m_light.Update();
		camera.Update();
		RenderShadowMap(graphics, camera);
		RenderScene(graphics, camera);
	}
}