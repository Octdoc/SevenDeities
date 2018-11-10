#include "gfx_simplerenderer.h"

namespace octdoc
{
	namespace gfx
	{
		void SimpleRenderer::CreateRenderer(Graphics::P graphics)
		{
			ID3D11Device* device = graphics->getDevice();
			m_ambient = 0.5f;
			m_vertexShader = gfx::VertexShader::Create(device, L"Shaders/vsSimple.cso", 
				(UINT)gfx::ShaderInputLayoutType::POSITION | (UINT)gfx::ShaderInputLayoutType::TEXCOORD |
				(UINT)gfx::ShaderInputLayoutType::NORMAL | (UINT)gfx::ShaderInputLayoutType::NORMALMAP);
			m_pixelShader = gfx::PixelShader::Create(device, L"Shaders/psSimple.cso");
			m_vsMatrixBuffer = gfx::CBuffer::Create(device, sizeof(mth::float4x4) * 2);
			m_psLightBuffer = gfx::CBuffer::Create(device, sizeof(float) * 8);
			m_sampler = gfx::SamplerState::Create(device, true);
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
}