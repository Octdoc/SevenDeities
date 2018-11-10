#pragma once
#include "graphics/gfx_graphics.h"

namespace octdoc
{
	namespace gfx
	{
		class SamplerState
		{
			SHARED_ONLY(SamplerState)
			AutoReleasePtr<ID3D11SamplerState> m_sampler;

		private:
			SamplerState(ID3D11Device* device, bool interpolate, D3D11_TEXTURE_ADDRESS_MODE addressMode);

			void FillSamplerDesc(D3D11_SAMPLER_DESC& samplerDesc, bool interpolate, D3D11_TEXTURE_ADDRESS_MODE addressMode);
			void CreateSamplerState(ID3D11Device* device, bool interpolate = true,
				D3D11_TEXTURE_ADDRESS_MODE addressMode = D3D11_TEXTURE_ADDRESS_WRAP);

		public:
			static SamplerState::P Create(ID3D11Device* device, bool interpolate = true,
				D3D11_TEXTURE_ADDRESS_MODE addressMode = D3D11_TEXTURE_ADDRESS_WRAP);

			void SetSamplerStateToRender(ID3D11DeviceContext* deviceContext, int index = 0);
		};

		class Texture
		{
			SHARED_ONLY(Texture)

		protected:
			AutoReleasePtr<ID3D11Texture2D> m_texture;
			AutoReleasePtr<ID3D11ShaderResourceView> m_shaderResourceView;
			int m_width, m_height;
			bool m_cube;

		protected:
			Texture() = default;
			Texture(ID3D11Device* device, const WCHAR* filename, bool cube);

			void FillTextureDesc(D3D11_TEXTURE2D_DESC& textureDesc);
			void FillShaderResourceViewDesc(D3D11_SHADER_RESOURCE_VIEW_DESC& shaderResourceViewDesc);
			void CreateTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, unsigned char* data);
			void CreateShaderResourceView(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

			void LoadTexture2D(ID3D11Device* device, const WCHAR* filename);
			void LoadTextureCube(ID3D11Device* device, const WCHAR* filename);

		public:
			static Texture::P Create(ID3D11Device* device, const WCHAR* filename, bool cubic);
			static Texture::P Create2D(ID3D11Device* device, const WCHAR* filename);
			static Texture::P CreateCube(ID3D11Device* device, const WCHAR* filename);

			void SetTextureToRender(ID3D11DeviceContext* deviceContext, int index = 0);

			int getWidth();
			int getHeight();
		};

		class RenderTarget :public Texture
		{
			SHARED_ONLY(RenderTarget)

			AutoReleasePtr<ID3D11RenderTargetView> m_renderTargetViews[6];
			AutoReleasePtr<ID3D11Texture2D> m_depthBuffer;
			AutoReleasePtr<ID3D11DepthStencilView> m_depthStencilView;

		private:
			RenderTarget(ID3D11Device* device, int width, int height, bool cube);

			void FillTextureDesc(D3D11_TEXTURE2D_DESC& textureDesc);
			void FillShaderResourceViewDesc(D3D11_SHADER_RESOURCE_VIEW_DESC& shaderResourceViewDesc);
			void FillDepthBufferDesc(D3D11_TEXTURE2D_DESC& depthBufferDesc);
			void FillDepthStencilViewDesc(D3D11_DEPTH_STENCIL_VIEW_DESC& depthStencilViewDesc);
			void FillRenderTargetViewDesc(D3D11_RENDER_TARGET_VIEW_DESC& renderTargetViewDesc);
			void CreateTexture(ID3D11Device* device);
			void CreateShaderResourceView(ID3D11Device* device);
			void CreateDepthBuffer(ID3D11Device* device);
			void CreateDepthStencilView(ID3D11Device* device);
			void CreateRenderTargetView(ID3D11Device* device);

			void CreateRenderTarget(ID3D11Device* device, int width, int height, bool cube);

		public:
			static RenderTarget::P Create(ID3D11Device* device, int width, int height, bool cube = false);

			ID3D11RenderTargetView* getRenderTargetView(UINT index = 0);
			ID3D11DepthStencilView* getDepthStencilView();
		};
	}
}