#include "gfw_colorrenderer.h"

namespace gfw
{
	void ColorRenderer::CreateRenderer(gfw::Graphics::P graphics)
	{
		ID3D11Device* device = graphics->getDevice();
		m_ambient = 0.5f;
		m_vertexShader = gfw::VertexShader::Create(device, L"Shaders/vsColor.cso", gfw::SIL_POSITION | gfw::SIL_NORMAL);
		m_pixelShader = gfw::PixelShader::Create(device, L"Shaders/psColor.cso");
		m_vsMatrixBuffer = gfw::CBuffer::Create(device, sizeof(mth::float4x4) * 2);
		m_psLightBuffer = gfw::CBuffer::Create(device, sizeof(float) * 8);
		m_psColorBuffer = gfw::CBuffer::Create(device, sizeof(float) * 4);
	}
	ColorRenderer::ColorRenderer() :m_ambient(0.0f) {}
	ColorRenderer::ColorRenderer(gfw::Graphics::P graphics) : m_ambient(0.0f)
	{
		CreateRenderer(graphics);
	}
	ColorRenderer::P ColorRenderer::Create()
	{
		return std::make_shared<ColorRenderer>();
	}
	ColorRenderer::P ColorRenderer::Create(gfw::Graphics::P graphics)
	{
		return std::make_shared<ColorRenderer>(graphics);
	}
	void ColorRenderer::Render(gfw::Graphics::P graphics, gfw::Camera& camera)
	{
		ID3D11DeviceContext* deviceContext = graphics->getDeviceContext();
		camera.Update();

		graphics->RenderToScreen();

		if (m_sky)
			m_sky->Render(deviceContext, camera);

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
		gfw::PixelShader::SetCBuffer(deviceContext, m_psLightBuffer, 0);
		gfw::PixelShader::SetCBuffer(deviceContext, m_psColorBuffer, 1);

		for (auto& e : m_entities)
		{
			matrixBuffer[0] = mth::float4x4::Identity();
			e->RenderWithSubparts(deviceContext, matrixBuffer, m_vsMatrixBuffer, m_psColorBuffer);
		}

		graphics->Present();
	}
}