#include "gfx_shader.h"
#include <fstream>

namespace octdoc
{
	namespace gfx
	{
#pragma region CBuffer

		void CBuffer::FillBufferDesc(D3D11_BUFFER_DESC& bufferDesc)
		{
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.ByteWidth = m_bufferSize;
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}

		CBuffer::P CBuffer::Create(ID3D11Device* device, UINT sizeInBytes)
		{
			return std::make_shared<CBuffer>(device, sizeInBytes);
		}
		CBuffer::CBuffer(ID3D11Device* device, UINT sizeInBytes) : m_bufferSize(0)
		{
			CreateCBuffer(device, sizeInBytes);
		}

		void CBuffer::CreateCBuffer(ID3D11Device* device, UINT sizeInBytes)
		{
			D3D11_BUFFER_DESC bufferDesc;
			HRESULT result;

			m_bufferSize = sizeInBytes;
			FillBufferDesc(bufferDesc);

			if (FAILED(result = device->CreateBuffer(&bufferDesc, NULL, &m_buffer)))
				throw hcs::Exception(L"Failed To Create Constant Buffer", result);
		}
		bool CBuffer::WriteBuffer(ID3D11DeviceContext* deviceContext, void* dataptr)
		{
			D3D11_MAPPED_SUBRESOURCE resource;
			if (FAILED(deviceContext->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource)))
				return false;
			memcpy(resource.pData, dataptr, m_bufferSize);
			deviceContext->Unmap(m_buffer, 0);
			return true;
		}
		ID3D11Buffer* CBuffer::getBuffer()
		{
			return m_buffer;
		}
		UINT CBuffer::getBufferSize()
		{
			return m_bufferSize;
		}

#pragma endregion

		ID3DBlob* LoadShaderCode(const WCHAR* filename)
		{
			HRESULT result;
			std::fstream f(filename, std::ios::in | std::ios::binary);
			if (!f.good())
				throw hcs::Exception::FileLoadingException(filename);
			auto begin = f.tellg();
			f.seekg(0, std::ios::end);
			auto end = f.tellg();
			f.seekg(0);
			ID3DBlob* shaderCode = nullptr;
			result = D3DCreateBlob(end - begin, &shaderCode);
			if (FAILED(result))
				throw hcs::Exception(L"Failed To Create Blob", result);
			f.read((char*)shaderCode->GetBufferPointer(), shaderCode->GetBufferSize());
			f.close();
			return shaderCode;
		}

#pragma region Vertex size functions

		UINT VertexSizeInBytes(UINT shaderInputLayoutType)
		{
			return VertexSizeInFloats(shaderInputLayoutType) * sizeof(float);
		}

		UINT VertexSizeInFloats(UINT shaderInputLayoutType)
		{
			UINT size = 0;
			if (shaderInputLayoutType & (UINT)ShaderInputLayoutType::POSITION)
				size += 3;
			if (shaderInputLayoutType & (UINT)ShaderInputLayoutType::COLOR)
				size += 4;
			if (shaderInputLayoutType & ((UINT)ShaderInputLayoutType::TEXCOORD | (UINT)ShaderInputLayoutType::NORMALMAP))
				size += 2;
			if (shaderInputLayoutType & (UINT)ShaderInputLayoutType::NORMAL)
				size += 3;
			if (shaderInputLayoutType & (UINT)ShaderInputLayoutType::NORMALMAP)
				size += 3 + 3;
			return size;
		}

		UINT PositionOffset(UINT shaderInputLayoutType)
		{
			UINT offset = 0;
			return offset;
		}

		UINT ColorOffset(UINT shaderInputLayoutType)
		{
			UINT offset = 0;
			if (shaderInputLayoutType & (UINT)ShaderInputLayoutType::POSITION)
				offset += 3;
			return offset;
		}

		UINT TexcoordOffset(UINT shaderInputLayoutType)
		{
			UINT offset = 0;
			if (shaderInputLayoutType & (UINT)ShaderInputLayoutType::POSITION)
				offset += 3;
			if (shaderInputLayoutType & (UINT)ShaderInputLayoutType::COLOR)
				offset += 4;
			return offset;
		}

		UINT NormalOffset(UINT shaderInputLayoutType)
		{
			UINT offset = 0;
			if (shaderInputLayoutType & (UINT)ShaderInputLayoutType::POSITION)
				offset += 3;
			if (shaderInputLayoutType & (UINT)ShaderInputLayoutType::COLOR)
				offset += 4;
			if (shaderInputLayoutType & ((UINT)ShaderInputLayoutType::TEXCOORD | (UINT)ShaderInputLayoutType::NORMALMAP))
				offset += 2;
			return offset;
		}

		UINT TangentOffset(UINT shaderInputLayoutType)
		{
			UINT offset = 0;
			if (shaderInputLayoutType & (UINT)ShaderInputLayoutType::POSITION)
				offset += 3;
			if (shaderInputLayoutType & (UINT)ShaderInputLayoutType::COLOR)
				offset += 4;
			if (shaderInputLayoutType & ((UINT)ShaderInputLayoutType::TEXCOORD | (UINT)ShaderInputLayoutType::NORMALMAP))
				offset += 2;
			if (shaderInputLayoutType & (UINT)ShaderInputLayoutType::NORMAL)
				offset += 3;
			return offset;
		}

		UINT BinormalOffset(UINT shaderInputLayoutType)
		{
			UINT offset = 0;
			if (shaderInputLayoutType & (UINT)ShaderInputLayoutType::POSITION)
				offset += 3;
			if (shaderInputLayoutType & (UINT)ShaderInputLayoutType::COLOR)
				offset += 4;
			if (shaderInputLayoutType & ((UINT)ShaderInputLayoutType::TEXCOORD | (UINT)ShaderInputLayoutType::NORMALMAP))
				offset += 2;
			if (shaderInputLayoutType & (UINT)ShaderInputLayoutType::NORMAL)
				offset += 3;
			if (shaderInputLayoutType & (UINT)ShaderInputLayoutType::NORMALMAP)
				offset += 3;
			return offset;
		}

#pragma endregion

#pragma region VertexShader

		int VertexShader::FillInputLayoutDesc(D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[])
		{
			int counter = 0;
			if (m_inputLayoutType & (UINT)ShaderInputLayoutType::POSITION)
				inputLayoutDesc[counter++] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			if (m_inputLayoutType & (UINT)ShaderInputLayoutType::COLOR)
				inputLayoutDesc[counter++] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			if (m_inputLayoutType & ((UINT)ShaderInputLayoutType::TEXCOORD | (UINT)ShaderInputLayoutType::NORMALMAP))
				inputLayoutDesc[counter++] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			if (m_inputLayoutType & (UINT)ShaderInputLayoutType::NORMAL)
				inputLayoutDesc[counter++] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			if (m_inputLayoutType & (UINT)ShaderInputLayoutType::NORMALMAP)
			{
				inputLayoutDesc[counter++] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
				inputLayoutDesc[counter++] = { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			}
			return counter;
		}

		void VertexShader::CreateVertexShader(ID3D11Device* device, ID3DBlob* shaderBuffer)
		{
			HRESULT result = device->CreateVertexShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &m_shader);
			if (FAILED(result))
				throw hcs::Exception(L"Failed To Create Vertex Shader.", result);
		}

		void VertexShader::CreateInputLayout(ID3D11Device* device, ID3DBlob* shaderBuffer)
		{
			D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[6];
			int inputLayoutSize = FillInputLayoutDesc(inputLayoutDesc);
			HRESULT result = device->CreateInputLayout(inputLayoutDesc, inputLayoutSize,
				shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), &m_inputLayout);
			if (FAILED(result))
				throw hcs::Exception(L"Failed To Create Input Layout.", result);
		}

		VertexShader::P VertexShader::Create(ID3D11Device* device, const WCHAR* shaderFileName, UINT inputLayoutType)
		{
			return std::make_shared<VertexShader>(device, shaderFileName, inputLayoutType);
		}
		VertexShader::VertexShader(ID3D11Device* device, const WCHAR* shaderFileName, UINT inputLayoutType) :m_inputLayout(0)
		{
			LoadVertexShader(device, shaderFileName, inputLayoutType);
		}

		void VertexShader::LoadVertexShader(ID3D11Device* device, const WCHAR* shaderFileName, UINT inputLayoutType)
		{
			AutoReleasePtr<ID3DBlob> shaderBuffer = LoadShaderCode(shaderFileName);
			CreateVertexShader(device, shaderBuffer);
			m_inputLayoutType = inputLayoutType;
			CreateInputLayout(device, shaderBuffer);
		}
		void VertexShader::SetShaderToRender(ID3D11DeviceContext* deviceContext)
		{
			deviceContext->IASetInputLayout(m_inputLayout);
			deviceContext->VSSetShader(m_shader, nullptr, 0);
		}
		void VertexShader::SetCBuffer(ID3D11DeviceContext* deviceContext, CBuffer::P buffer, UINT index)
		{
			ID3D11Buffer* bufferPtr = buffer->getBuffer();
			deviceContext->VSSetConstantBuffers(index, 1, &bufferPtr);
		}

#pragma endregion

#pragma region PixelShader

		PixelShader::PixelShader(ID3D11Device* device, const WCHAR* shaderFileName)
		{
			LoadPixelShader(device, shaderFileName);
		}
		PixelShader::P PixelShader::Create(ID3D11Device* device, const WCHAR* shaderFileName)
		{
			return std::make_shared<PixelShader>(device, shaderFileName);
		}

		void PixelShader::LoadPixelShader(ID3D11Device* device, const WCHAR* shaderFileName)
		{
			HRESULT result;
			AutoReleasePtr<ID3DBlob> shaderBuffer = LoadShaderCode(shaderFileName);
			if (shaderBuffer == nullptr)
				throw hcs::Exception::FileLoadingException(shaderFileName);
			result = device->CreatePixelShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), nullptr, &m_shader);
			if (FAILED(result))
				throw hcs::Exception(L"Failed To Create Pixel Shader.", result);
		}
		void PixelShader::SetShaderToRender(ID3D11DeviceContext* deviceContext)
		{
			deviceContext->PSSetShader(m_shader, nullptr, 0);
		}
		void PixelShader::SetCBuffer(ID3D11DeviceContext* deviceContext, CBuffer::P buffer, UINT index)
		{
			ID3D11Buffer* bufferPtr = buffer->getBuffer();
			deviceContext->PSSetConstantBuffers(index, 1, &bufferPtr);
		}

#pragma endregion

	}
}