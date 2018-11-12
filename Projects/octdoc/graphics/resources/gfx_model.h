#pragma once

#include "gfx_modelloader.h"

namespace octdoc
{
	namespace gfx
	{
		class Model
		{
			SHARED_ONLY(Model)

		protected:
			AutoReleasePtr<ID3D11Buffer> m_vertexBuffer;
			AutoReleasePtr<ID3D11Buffer> m_indexBuffer;

			UINT m_vertexSizeInBytes;
			UINT m_shaderInputLayout;
			UINT m_vertexCount;
			UINT m_indexCount;

		private:
			Model(ID3D11Device* device, const WCHAR* filename);
			Model(ID3D11Device* device, ModelLoader& modelLoader);
			Model(ID3D11Device* device, void* vertices, UINT vertexCount, UINT shaderInputLayout, UINT* indices, UINT indexCount);

			void CreateVertexBuffer(ID3D11Device* device, void* vertices, UINT vertexCount, UINT shaderInputLayout);
			void FillVertexBufferDesc(D3D11_BUFFER_DESC& bufferDesc);
			void CreateIndexBuffer(ID3D11Device* device, UINT* indices, UINT indexCount);
			void FillIndexBufferDesc(D3D11_BUFFER_DESC& bufferDesc);
			void SetSubresourceData(D3D11_SUBRESOURCE_DATA& data, void* ptr);

			void LoadModel(ID3D11Device* device, const WCHAR* filename);
			void CreateModel(ID3D11Device* device, ModelLoader& modelLoader);
			void CreateModel(ID3D11Device* device, void* vertices, UINT vertexCount, UINT shaderInputLayout, UINT* indices, UINT indexCount);

		public:
			static Model::P Create(ID3D11Device* device, const WCHAR* filename);
			static Model::P Create(ID3D11Device* device, ModelLoader& modelLoader);
			static Model::P Create(ID3D11Device* device, void* vertices, UINT vertexCount, UINT shaderInputLayout, UINT* indices, UINT indexCount);

			UINT getVertexCount();
			UINT getIndexCount();
			UINT getShaderInputLayout();

			void Render(ID3D11DeviceContext* deviceContext);
		};
	}
}