#include "gfx_model.h"
#include "gfx_shader.h"
#include <fstream>

namespace octdoc
{
	namespace gfx
	{
		void Model::CreateVertexBuffer(ID3D11Device* device, void* vertices, UINT vertexCount, UINT shaderInputLayout)
		{
			HRESULT result;
			D3D11_BUFFER_DESC bufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData;

			m_vertexSizeInBytes = VertexSizeInBytes(shaderInputLayout);
			m_shaderInputLayout = shaderInputLayout;
			m_vertexCount = vertexCount;

			FillVertexBufferDesc(bufferDesc);
			SetSubresourceData(vertexData, vertices);

			result = device->CreateBuffer(&bufferDesc, &vertexData, &m_vertexBuffer);
			if (FAILED(result))
				throw hcs::Exception(L"Failed To Create Vertex Buffer", result);
		}
		void Model::FillVertexBufferDesc(D3D11_BUFFER_DESC& bufferDesc)
		{
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.ByteWidth = m_vertexCount * m_vertexSizeInBytes;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
		}
		void Model::CreateIndexBuffer(ID3D11Device* device, UINT* indices, UINT indexCount)
		{
			HRESULT result;
			D3D11_BUFFER_DESC bufferDesc;
			D3D11_SUBRESOURCE_DATA indexData;

			m_indexCount = indexCount;

			FillIndexBufferDesc(bufferDesc);
			SetSubresourceData(indexData, indices);

			result = device->CreateBuffer(&bufferDesc, &indexData, &m_indexBuffer);
			if (FAILED(result))
				throw hcs::Exception(L"Failed To Create Index Buffer", result);
		}
		void Model::FillIndexBufferDesc(D3D11_BUFFER_DESC& bufferDesc)
		{
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.ByteWidth = m_indexCount * sizeof(UINT);
			bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
		}

		void Model::SetSubresourceData(D3D11_SUBRESOURCE_DATA& data, void* ptr)
		{
			data.pSysMem = ptr;
			data.SysMemPitch = 0;
			data.SysMemSlicePitch = 0;
		}

		Model::Model(ID3D11Device* device, const WCHAR* filename) : m_vertexCount(0), m_vertexSizeInBytes(0), m_indexCount(0)
		{
			LoadModel(device, filename);
		}
		Model::Model(ID3D11Device* device, ModelLoader& modelLoader) : m_vertexCount(0), m_vertexSizeInBytes(0), m_indexCount(0)
		{
			CreateModel(device, modelLoader);
		}
		Model::Model(ID3D11Device* device, void* vertices, UINT vertexCount, UINT shaderInputLayout, UINT* indices, UINT indexCount) : m_vertexCount(0), m_vertexSizeInBytes(0), m_indexCount(0)
		{
			CreateModel(device, vertices, vertexCount, shaderInputLayout, indices, indexCount);
		}
		Model::P Model::Create(ID3D11Device* device, const WCHAR* filename)
		{
			return std::make_shared<Model>(device, filename);
		}
		Model::P Model::Create(ID3D11Device* device, ModelLoader& modelLoader)
		{
			return std::make_shared<Model>(device, modelLoader);
		}
		Model::P Model::Create(ID3D11Device* device, void* vertices, UINT vertexCount, UINT shaderInputLayout, UINT* indices, UINT indexCount)
		{
			return std::make_shared<Model>(device, vertices, vertexCount, shaderInputLayout, indices, indexCount);
		}

		void Model::LoadModel(ID3D11Device* device, const WCHAR* filename)
		{
			ModelLoader modelLoader(filename);
			CreateModel(device, modelLoader);
		}

		void Model::CreateModel(ID3D11Device* device, ModelLoader& modelLoader)
		{
			CreateModel(device, modelLoader.getVertices(), modelLoader.getVertexCount(), modelLoader.getShaderInputLayout(), modelLoader.getIndices(), modelLoader.getIndexCount());
		}

		void Model::CreateModel(ID3D11Device* device, void* vertices, UINT vertexCount, UINT shaderInputLayout, UINT* indices, UINT indexCount)
		{
			m_indexGroupStarts.push_back(0);
			m_indexGroupSizes.push_back(indexCount);
			CreateVertexBuffer(device, vertices, vertexCount, shaderInputLayout);
			CreateIndexBuffer(device, indices, indexCount);
		}

		UINT Model::getIndexGroupCount()
		{
			return (UINT)m_indexGroupSizes.size();
		}

		UINT Model::getVertexCount()
		{
			return m_vertexCount;
		}

		UINT Model::getIndexCount()
		{
			return m_indexCount;
		}

		UINT Model::getShaderInputLayout()
		{
			return m_shaderInputLayout;
		}

		void Model::Render(ID3D11DeviceContext* deviceContext)
		{
			SetBuffersToRender(deviceContext);
			deviceContext->DrawIndexed(m_indexCount, 0, 0);
		}
		void Model::RenderPart(ID3D11DeviceContext* deviceContext, UINT index)
		{
			SetBuffersToRender(deviceContext);
			deviceContext->DrawIndexed(m_indexGroupSizes[index], m_indexGroupSizes[index], 0);
		}
		void Model::SetBuffersToRender(ID3D11DeviceContext* deviceContext)
		{
			UINT stride = m_vertexSizeInBytes;
			UINT offset = 0;

			deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
			deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		}
		void Model::RenderPart_DrawCallOnly(ID3D11DeviceContext* deviceContext, UINT index)
		{
			deviceContext->DrawIndexed(m_indexGroupSizes[index], m_indexGroupStarts[index], 0);
		}
	}
}