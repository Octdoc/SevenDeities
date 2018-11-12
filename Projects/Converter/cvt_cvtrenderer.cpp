#include "cvt_cvtrenderer.h"

using namespace octdoc;

namespace cvt
{
	void CVTRenderer::CreateRenderer(gfx::Graphics::P graphics)
	{
		ID3D11Device* device = graphics->getDevice();
		m_ambient = 0.5f;
		m_vertexShader = gfx::VertexShader::Create(device, L"Shaders/vsConverter.cso",
			(UINT)gfx::ShaderInputLayoutType::POSITION |
			(UINT)gfx::ShaderInputLayoutType::COLOR |
			(UINT)gfx::ShaderInputLayoutType::TEXCOORD |
			(UINT)gfx::ShaderInputLayoutType::NORMAL |
			(UINT)gfx::ShaderInputLayoutType::NORMALMAP);
		m_pixelShader = gfx::PixelShader::Create(device, L"Shaders/psConverter.cso");
		m_vsMatrixBuffer = gfx::CBuffer::Create(device, sizeof(mth::float4x4) * 2);
		m_psLightBuffer = gfx::CBuffer::Create(device, sizeof(float) * 8);
		m_sampler = gfx::SamplerState::Create(device, true);
	}
	CVTRenderer::CVTRenderer() :m_ambient(0.0f) {}
	CVTRenderer::CVTRenderer(gfx::Graphics::P graphics) : m_ambient(0.0f)
	{
		CreateRenderer(graphics);
	}
	CVTRenderer::P CVTRenderer::Create()
	{
		return std::make_shared<CVTRenderer>();
	}
	CVTRenderer::P CVTRenderer::Create(gfx::Graphics::P graphics)
	{
		return std::make_shared<CVTRenderer>(graphics);
	}
	void CVTRenderer::Render(gfx::Graphics::P graphics, gfx::Camera& camera)
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
		gfx::VertexShader::SetCBuffer(deviceContext, m_vsMatrixBuffer);

		mth::float3 campos = camera.position;
		float lightBuffer[8] = {
			1.0f, 1.0f, 1.0f, 1.0f,
			campos.x, campos.y, campos.z,
			m_ambient
		};
		m_psLightBuffer->WriteBuffer(deviceContext, lightBuffer);
		gfx::PixelShader::SetCBuffer(deviceContext, m_psLightBuffer);

		for (auto& e : m_entities)
		{
			matrixBuffer[0] = mth::float4x4::Identity();
			e->RenderWithSubparts(deviceContext, matrixBuffer, m_vsMatrixBuffer);
		}

		graphics->Present();
	}
}