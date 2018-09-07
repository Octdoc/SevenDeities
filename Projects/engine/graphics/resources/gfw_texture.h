#pragma once
#include "graphics/gfw_graphics.h"

namespace gfw
{
	class SamplerState :public Resource
	{
		AutoReleasePtr<ID3D11SamplerState> m_sampler;

	private:
		void FillSamplerDesc(D3D11_SAMPLER_DESC& samplerDesc, bool interpolate, D3D11_TEXTURE_ADDRESS_MODE addressMode);

	public:
		SamplerState();
		SamplerState(SamplerState&) = delete;
		SamplerState(ID3D11Device* device, bool interpolate, D3D11_TEXTURE_ADDRESS_MODE addressMode);
		static std::shared_ptr<SamplerState> Create();
		static std::shared_ptr<SamplerState> Create(ID3D11Device* device, bool interpolate = true, D3D11_TEXTURE_ADDRESS_MODE addressMode = D3D11_TEXTURE_ADDRESS_WRAP);

		void CreateSamplerState(ID3D11Device* device, bool interpolate = true, D3D11_TEXTURE_ADDRESS_MODE addressMode = D3D11_TEXTURE_ADDRESS_WRAP);
		virtual void Release() override;
		void SetSamplerState(ID3D11DeviceContext* deviceContext, int index = 0);
	};

	class Texture :public Resource
	{
	protected:
		AutoReleasePtr<ID3D11Texture2D> m_texture;
		AutoReleasePtr<ID3D11ShaderResourceView> m_shaderResourceView;
		int m_width, m_height;
		bool m_cube;

	protected:
		void FillTextureDesc(D3D11_TEXTURE2D_DESC& textureDesc);
		void FillShaderResourceViewDesc(D3D11_SHADER_RESOURCE_VIEW_DESC& shaderResourceViewDesc);
		void CreateTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, unsigned char* data);
		void CreateShaderResourceView(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	public:
		Texture();
		Texture(ID3D11Device* device, const WCHAR* filename, bool cube);
		static std::shared_ptr<Texture> Create();
		static std::shared_ptr<Texture> Create2D(ID3D11Device* device, const WCHAR* filename);
		static std::shared_ptr<Texture> CreateCube(ID3D11Device* device, const WCHAR* filename);

		void LoadTexture2D(ID3D11Device* device, const WCHAR* filename);
		void LoadTextureCube(ID3D11Device* device, const WCHAR* filename);
		virtual void Release() override;

		void SetTexture(ID3D11DeviceContext* deviceContext, int index = 0);

		int getWidth();
		int getHeight();
	};

	class RenderTarget :public Texture
	{
		friend std::shared_ptr<RenderTarget>;

		AutoReleasePtr<ID3D11RenderTargetView> m_renderTargetViews[6];
		AutoReleasePtr<ID3D11Texture2D> m_depthBuffer;
		AutoReleasePtr<ID3D11DepthStencilView> m_depthStencilView;

	private:
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

	public:
		RenderTarget();
		RenderTarget(ID3D11Device* device, int width, int height, bool cube);
		static std::shared_ptr<RenderTarget> Create();
		static std::shared_ptr<RenderTarget> Create(ID3D11Device* device, int width, int height, bool cube = false);

		void CreateRenderTarget(ID3D11Device* device, int width, int height, bool cube = false);
		virtual void Release() override;

		ID3D11RenderTargetView* getRenderTargetView(UINT index = 0);
		ID3D11DepthStencilView* getDepthStencilView();
	};
}