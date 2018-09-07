#include "gfw_skydome.h"

namespace gfw
{
	SkyDome::SkyDome() {}
	SkyDome::SkyDome(ID3D11Device* device, const WCHAR* filename)
	{
		CreateSkyDome(device, filename);
	}
	std::shared_ptr<SkyDome> SkyDome::Create()
	{
		return std::make_shared<SkyDome>();
	}
	std::shared_ptr<SkyDome> SkyDome::Create(ID3D11Device* device, const WCHAR* filename)
	{
		return std::make_shared<SkyDome>(device, filename);
	}
	void SkyDome::CreateSkyDome(ID3D11Device* device, const WCHAR* filename)
	{
		ModelLoader ml;
		ml.CreateCube({ -1.0f, -1.0f, -1.0f }, { 2.0f, 2.0f, 2.0f }, SIL_POSITION);
		ml.FlipInsideOut();
		m_model = Model::Create(device, ml);
		m_vertexShader = VertexShader::Create(device, L"Shaders/vsSky.cso", SIL_POSITION);
		m_pixelShader = PixelShader::Create(device, L"Shaders/psSky.cso");
		m_cBuffer = CBuffer::Create(device, sizeof(mth::float4x4));
		m_texture = Texture::CreateCube(device, filename);
		m_sampler = SamplerState::Create(device);
	}
	void SkyDome::Render(ID3D11DeviceContext* deviceContext, Camera& camera)
	{
		m_sampler->SetSamplerState(deviceContext);
		m_vertexShader->SetShaderToRender(deviceContext);
		m_pixelShader->SetShaderToRender(deviceContext);
		mth::float4x4 matrixBuffer = camera.GetCameraMatrix()*mth::float4x4::Translation(camera.position);
		m_cBuffer->WriteBuffer(deviceContext, &matrixBuffer);
		VertexShader::SetCBuffer(deviceContext, *m_cBuffer);
		m_texture->SetTexture(deviceContext);
		m_model->Render(deviceContext);
	}
}