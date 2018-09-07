#pragma once

#include "graphics/resources/gfw_model.h"
#include "graphics/resources/gfw_texture.h"
#include "graphics/resources/gfw_shader.h"
#include "graphics/entity/gfw_camera.h"

namespace gfw
{
	class SkyDome
	{
		std::shared_ptr<Model> m_model;
		std::shared_ptr<VertexShader> m_vertexShader;
		std::shared_ptr<PixelShader> m_pixelShader;
		std::shared_ptr<CBuffer> m_cBuffer;
		std::shared_ptr<Texture> m_texture;
		std::shared_ptr<SamplerState> m_sampler;

	public:
		SkyDome();
		SkyDome(ID3D11Device* device, const WCHAR* filename);
		static std::shared_ptr<SkyDome> Create();
		static std::shared_ptr<SkyDome> Create(ID3D11Device* device, const WCHAR* filename);
		void CreateSkyDome(ID3D11Device* device, const WCHAR* filename);
		void Render(ID3D11DeviceContext* deviceContext, Camera& camera);
	};
}