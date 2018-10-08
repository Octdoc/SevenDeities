#include "gfw_simplerenderer.h"

namespace gfw
{
	void SimpleRenderer::CreateRenderer(Graphics::P graphics)
	{
		ID3D11Device* device = graphics->getDevice();
		m_ambient = 0.5f;
		m_vertexShader = gfw::VertexShader::Create(device, L"Shaders/vsSimple.cso", gfw::SIL_POSITION | gfw::SIL_TEXCOORD | gfw::SIL_NORMAL | gfw::SIL_NORMALMAP);
		m_pixelShader = gfw::PixelShader::Create(device, L"Shaders/psSimple.cso");
		m_vsMatrixBuffer = gfw::CBuffer::Create(device, sizeof(mth::float4x4) * 2);
		m_psLightBuffer = gfw::CBuffer::Create(device, sizeof(float) * 8);
		m_sampler = gfw::SamplerState::Create(device, true);
	}
	SimpleRenderer::SimpleRenderer() :m_ambient(0.0f) {}
	SimpleRenderer::SimpleRenderer(Graphics::P graphics) : m_ambient(0.0f)
	{
		CreateRenderer(graphics);
	}
	SimpleRenderer::P SimpleRenderer::Create()
	{
		return std::make_shared<SimpleRenderer>();
	}	
	SimpleRenderer::P SimpleRenderer::Create(Graphics::P graphics)
	{
		return std::make_shared<SimpleRenderer>(graphics);
	}
	void SimpleRenderer::Render(Graphics::P graphics, Camera& camera)
	{
		ID3D11DeviceContext* deviceContext = graphics->getDeviceContext();
		camera.Update();

		graphics->RenderToScreen();

		if (m_sky)
			m_sky->Render(deviceContext, camera);

		m_sampler->SetSamplerStateToRender(deviceContext);
		m_vertexShader->SetShaderToRender(deviceContext);
		m_pixelShader->SetShaderToRender(deviceContext);

		mth::float4x4 matrixBuffer[2];
		matrixBuffer[1] = camera.GetCameraMatrix();
		gfw::VertexShader::SetCBuffer(deviceContext, m_vsMatrixBuffer);

		mth::float3 campos = camera.position;
		float lightBuffer[8] = {
			1.0f, 1.0f, 1.0f, 1.0f,
			campos.x, campos.y, campos.z,
			m_ambient
		};
		m_psLightBuffer->WriteBuffer(deviceContext, lightBuffer);
		gfw::PixelShader::SetCBuffer(deviceContext, m_psLightBuffer);

		for (auto& e : m_entities)
		{
			matrixBuffer[0] = mth::float4x4::Identity();
			e->RenderWithSubparts(deviceContext, matrixBuffer, m_vsMatrixBuffer);
		}

		graphics->Present();
	}
}