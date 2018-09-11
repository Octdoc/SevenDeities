#pragma once

#include "graphics/resources/gfw_model.h"
#include "graphics/resources/gfw_texture.h"
#include "graphics/resources/gfw_shader.h"
#include "graphics/entity/gfw_camera.h"

namespace gfw
{
	class SkyDome
	{
		SHARED_ONLY(SkyDome);
		Model::P m_model;
		VertexShader::P m_vertexShader;
		PixelShader::P m_pixelShader;
		CBuffer::P m_cBuffer;
		Texture::P m_texture;
		SamplerState::P m_sampler;

	private:
		SkyDome(ID3D11Device* device, const WCHAR* filename);
		void CreateSkyDome(ID3D11Device* device, const WCHAR* filename);

	public:
		static SkyDome::P Create(ID3D11Device* device, const WCHAR* filename);
		void Render(ID3D11DeviceContext* deviceContext, Camera& camera);
	};
}