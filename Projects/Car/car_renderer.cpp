#include "car_renderer.h"

namespace car
{
	void Renderer::CreateRenderer(gfw::Graphics::P graphics)
	{
		ID3D11Device* device = graphics->getDevice();
		m_ambient = 0.5f;
		m_vertexShader = gfw::VertexShader::Create(device, L"Shaders/vsCar.cso", gfw::SIL_POSITION | gfw::SIL_NORMAL);
		m_pixelShader = gfw::PixelShader::Create(device, L"Shaders/psCar.cso");
		m_vsMatrixBuffer = gfw::CBuffer::Create(device, sizeof(mth::float4x4) * 2);
		m_psLightBuffer = gfw::CBuffer::Create(device, sizeof(float) * 8);
		m_psColorBuffer = gfw::CBuffer::Create(device, sizeof(float) * 4);
	}
	Renderer::Renderer() :m_ambient(0.0f) {}
	Renderer::Renderer(gfw::Graphics::P graphics) : m_ambient(0.0f)
	{
		CreateRenderer(graphics);
	}
	Renderer::P Renderer::Create()
	{
		return std::make_shared<Renderer>();
	}
	Renderer::P Renderer::Create(gfw::Graphics::P graphics)
	{
		return std::make_shared<Renderer>(graphics);
	}
	void Renderer::Render(gfw::Graphics::P graphics, gfw::Camera& camera)
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
			if (e->getRelativeTo())
				matrixBuffer[0] = e->getRelativeTo()->GetWorldMatrix()*e->GetWorldMatrix();
			else
				matrixBuffer[0] = e->GetWorldMatrix();
			m_vsMatrixBuffer->WriteBuffer(deviceContext, matrixBuffer);
			m_psColorBuffer->WriteBuffer(deviceContext, &e->getColor());
			e->Render(deviceContext);
		}

		graphics->Present();
	}
}