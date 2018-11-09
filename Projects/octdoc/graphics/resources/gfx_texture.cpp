#include "gfx_texture.h"
#include "DDSTextureLoader.h"
#include "DirectXTex.h"

namespace octdoc
{
	namespace gfx
	{
#pragma region SamplerState

		SamplerState::SamplerState(ID3D11Device* device, bool interpolate, D3D11_TEXTURE_ADDRESS_MODE addressMode)
		{
			CreateSamplerState(device, interpolate, addressMode);
		}

		void SamplerState::FillSamplerDesc(D3D11_SAMPLER_DESC& samplerDesc, bool interpolate, D3D11_TEXTURE_ADDRESS_MODE addressMode)
		{
			samplerDesc.Filter = interpolate ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
			samplerDesc.AddressU = addressMode;
			samplerDesc.AddressV = addressMode;
			samplerDesc.AddressW = addressMode;
			samplerDesc.MipLODBias = 0.0f;
			samplerDesc.MaxAnisotropy = 1;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			samplerDesc.BorderColor[0] = 0;
			samplerDesc.BorderColor[1] = 0;
			samplerDesc.BorderColor[2] = 0;
			samplerDesc.BorderColor[3] = 0;
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		}

		SamplerState::P SamplerState::Create(ID3D11Device* device, bool interpolate, D3D11_TEXTURE_ADDRESS_MODE addressMode)
		{
			return std::make_shared<SamplerState>(device, interpolate, addressMode);
		}

		void SamplerState::CreateSamplerState(ID3D11Device* device, bool interpolate, D3D11_TEXTURE_ADDRESS_MODE addressMode)
		{
			D3D11_SAMPLER_DESC samplerDesc;
			FillSamplerDesc(samplerDesc, interpolate, addressMode);
			HRESULT result = device->CreateSamplerState(&samplerDesc, &m_sampler);
			if (FAILED(result))
				throw hcs::Exception(L"Failed To Create Sampler State", result);
		}

		void SamplerState::SetSamplerStateToRender(ID3D11DeviceContext* deviceContext, int index)
		{
			deviceContext->PSSetSamplers(index, 1, &m_sampler);
		}

#pragma endregion

#pragma region Texture

		void Texture::FillTextureDesc(D3D11_TEXTURE2D_DESC& textureDesc)
		{
			textureDesc.Height = m_height;
			textureDesc.Width = m_width;
			textureDesc.MipLevels = 0;
			textureDesc.ArraySize = 1;
			textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		}

		void Texture::FillShaderResourceViewDesc(D3D11_SHADER_RESOURCE_VIEW_DESC& shaderResourceViewDesc)
		{
			shaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
			shaderResourceViewDesc.Texture2D.MipLevels = -1;
		}

		void Texture::CreateTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, unsigned char* data)
		{
			HRESULT result;
			D3D11_TEXTURE2D_DESC textureDesc;
			FillTextureDesc(textureDesc);
			result = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
			if (FAILED(result))
				throw hcs::Exception(L"Failed To Create Texture", result);
			deviceContext->UpdateSubresource(m_texture, 0, NULL, data, m_width * 4 * sizeof(unsigned char), 0);
		}

		void Texture::CreateShaderResourceView(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
		{
			HRESULT result;
			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
			FillShaderResourceViewDesc(shaderResourceViewDesc);
			result = device->CreateShaderResourceView(m_texture, &shaderResourceViewDesc, &m_shaderResourceView);
			if (FAILED(result))
				throw hcs::Exception(L"Failed To Create Shader Resource View", result);
			deviceContext->GenerateMips(m_shaderResourceView);
		}

		Texture::P Texture::Create(ID3D11Device * device, const WCHAR * filename, bool cubic)
		{
			return std::make_shared<Texture>(device, filename, cubic);
		}
		Texture::P Texture::Create2D(ID3D11Device* device, const WCHAR* filename)
		{
			return std::make_shared<Texture>(device, filename, false);
		}
		Texture::P Texture::CreateCube(ID3D11Device* device, const WCHAR* filename)
		{
			return std::make_shared<Texture>(device, filename, true);
		}

		Texture::Texture(ID3D11Device* device, const WCHAR* filename, bool cube) : m_width(0), m_height(0)
		{
			if (cube)
				LoadTextureCube(device, filename);
			else
				LoadTexture2D(device, filename);
		}

		void Texture::LoadTexture2D(ID3D11Device* device, const WCHAR* filename)
		{
			HRESULT result;
			DirectX::TexMetadata imageData;
			DirectX::ScratchImage image, mipmap;
			result = DirectX::LoadFromWICFile(filename, 0, &imageData, image);
			if (FAILED(result))
				throw hcs::Exception(std::wstring(L"Failed to load file: ") + std::wstring(filename), result);
			result = DirectX::GenerateMipMaps(image.GetImage(0, 0, 0), 1, imageData, (DWORD)DirectX::TEX_FILTER_DEFAULT, 0, mipmap);
			if (FAILED(result))
				throw hcs::Exception(L"Failed to generate texture mipmaps.", result);
			result = DirectX::CreateTextureEx(device, mipmap.GetImages(), mipmap.GetImageCount(),
				imageData, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, false, (ID3D11Resource**)&m_texture);
			if (FAILED(result))
				throw hcs::Exception(L"Failed to create texture.", result);
			m_width = (int)imageData.width;
			m_height = (int)imageData.height;

			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
			ClearStruct(shaderResourceViewDesc);
			D3D11_TEXTURE2D_DESC textureDesc;
			m_texture->GetDesc(&textureDesc);
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDesc.Format = textureDesc.Format;
			shaderResourceViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;
			shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
			result = device->CreateShaderResourceView(m_texture, &shaderResourceViewDesc, &m_shaderResourceView);
			if (FAILED(result))
				throw hcs::Exception(L"Failed to create shader resource view.", result);
		}

		void Texture::LoadTextureCube(ID3D11Device* device, const WCHAR* filename)
		{
			HRESULT result = DirectX::CreateDDSTextureFromFile(device, filename, (ID3D11Resource**)&m_texture, &m_shaderResourceView);
			if (FAILED(result))
				throw hcs::Exception((std::wstring(L"Failed to load and create texture: ") + filename).c_str(), result);
		}

		void Texture::SetTextureToRender(ID3D11DeviceContext* deviceContext, int index)
		{
			deviceContext->PSSetShaderResources(index, 1, &m_shaderResourceView);
		}

		int Texture::getWidth()
		{
			return m_width;
		}

		int Texture::getHeight()
		{
			return m_height;
		}

#pragma endregion

#pragma region RenderTarget

		RenderTarget::RenderTarget(ID3D11Device* device, int width, int height, bool cube)
		{
			CreateRenderTarget(device, width, height, cube);
		}

		void RenderTarget::FillTextureDesc(D3D11_TEXTURE2D_DESC& textureDesc)
		{
			ClearStruct(textureDesc);
			textureDesc.Height = m_height;
			textureDesc.Width = m_width;
			textureDesc.MipLevels = 1;
			textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
			textureDesc.CPUAccessFlags = 0;
			if (m_cube)
			{
				textureDesc.ArraySize = 6;
				textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
			}
			else
			{
				textureDesc.ArraySize = 1;
				textureDesc.MiscFlags = 0;
			}
		}

		void RenderTarget::FillShaderResourceViewDesc(D3D11_SHADER_RESOURCE_VIEW_DESC& shaderResourceViewDesc)
		{
			ClearStruct(shaderResourceViewDesc);
			shaderResourceViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			if (m_cube)
			{
				shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
				shaderResourceViewDesc.TextureCube.MipLevels = 1;
				shaderResourceViewDesc.TextureCube.MostDetailedMip = 0;
			}
			else
			{
				shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				shaderResourceViewDesc.Texture2D.MipLevels = 1;
				shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
			}
		}

		void RenderTarget::FillDepthBufferDesc(D3D11_TEXTURE2D_DESC& depthBufferDesc)
		{
			ClearStruct(depthBufferDesc);
			depthBufferDesc.Width = m_width;
			depthBufferDesc.Height = m_height;
			depthBufferDesc.MipLevels = 1;
			depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthBufferDesc.SampleDesc.Count = 1;
			depthBufferDesc.SampleDesc.Quality = 0;
			depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			depthBufferDesc.CPUAccessFlags = 0;
			depthBufferDesc.MiscFlags = 0;
			depthBufferDesc.ArraySize = 1;
		}

		void RenderTarget::FillDepthStencilViewDesc(D3D11_DEPTH_STENCIL_VIEW_DESC& depthStencilViewDesc)
		{
			ClearStruct(depthStencilViewDesc);
			depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilViewDesc.Flags = 0;
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			depthStencilViewDesc.Texture2D.MipSlice = 0;
		}

		void RenderTarget::FillRenderTargetViewDesc(D3D11_RENDER_TARGET_VIEW_DESC& renderTargetViewDesc)
		{
			ClearStruct(renderTargetViewDesc);
			renderTargetViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			if (m_cube)
			{
				renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
				renderTargetViewDesc.Texture2DArray.MipSlice = 0;
				renderTargetViewDesc.Texture2DArray.ArraySize = 1;
			}
			else
			{
				renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
				renderTargetViewDesc.Texture2D.MipSlice = 0;
			}
		}

		void RenderTarget::CreateTexture(ID3D11Device* device)
		{
			D3D11_TEXTURE2D_DESC textureDesc;
			RenderTarget::FillTextureDesc(textureDesc);
			HRESULT result = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
			if (FAILED(result))
				throw hcs::Exception(L"Failed To Create Texture", result);
		}

		void RenderTarget::CreateShaderResourceView(ID3D11Device* device)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
			RenderTarget::FillShaderResourceViewDesc(shaderResourceViewDesc);
			HRESULT result = device->CreateShaderResourceView(m_texture, &shaderResourceViewDesc, &m_shaderResourceView);
			if (FAILED(result))
				throw hcs::Exception(L"Failed To Create Shader Resource View", result);
		}

		void RenderTarget::CreateDepthBuffer(ID3D11Device* device)
		{
			D3D11_TEXTURE2D_DESC depthBufferDesc;
			RenderTarget::FillDepthBufferDesc(depthBufferDesc);
			HRESULT result = device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthBuffer);
			if (FAILED(result))
				throw hcs::Exception(L"Failed To Create Depth Buffer", result);
		}

		void RenderTarget::CreateDepthStencilView(ID3D11Device* device)
		{
			D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
			RenderTarget::FillDepthStencilViewDesc(depthStencilViewDesc);
			HRESULT result = device->CreateDepthStencilView(m_depthBuffer, &depthStencilViewDesc, &m_depthStencilView);
			if (FAILED(result))
				throw hcs::Exception(L"Failed To Create Depth Stencil View", result);
		}

		void RenderTarget::CreateRenderTargetView(ID3D11Device* device)
		{
			HRESULT result;
			D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
			RenderTarget::FillRenderTargetViewDesc(renderTargetViewDesc);
			if (m_cube)
			{
				for (int i = 0; i < 6; i++)
				{
					renderTargetViewDesc.Texture2DArray.FirstArraySlice = i;
					result = device->CreateRenderTargetView(m_texture, &renderTargetViewDesc, &m_renderTargetViews[i]);
					if (FAILED(result))
						throw hcs::Exception(L"Failed To Create Render Target View", result);
				}
			}
			else
			{
				result = device->CreateRenderTargetView(m_texture, &renderTargetViewDesc, &m_renderTargetViews[0]);
				if (FAILED(result))
					throw hcs::Exception(L"Failed To Create Render Target View", result);
			}
		}

		RenderTarget::P RenderTarget::Create(ID3D11Device* device, int width, int height, bool cube)
		{
			return std::make_shared<RenderTarget>(device, width, height, cube);
		}

		void RenderTarget::CreateRenderTarget(ID3D11Device* device, int width, int height, bool cube)
		{
			m_width = width;
			m_height = height;
			m_cube = cube;

			RenderTarget::CreateTexture(device);
			RenderTarget::CreateShaderResourceView(device);
			RenderTarget::CreateDepthBuffer(device);
			RenderTarget::CreateDepthStencilView(device);
			RenderTarget::CreateRenderTargetView(device);
		}

		ID3D11RenderTargetView* RenderTarget::getRenderTargetView(UINT index)
		{
			return m_renderTargetViews[index];
		}

		ID3D11DepthStencilView* RenderTarget::getDepthStencilView()
		{
			return m_depthStencilView;
		}

#pragma endregion
	}
}