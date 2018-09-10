#pragma once

#include "graphics/gfw_graphics.h"

namespace gfw
{
	class CBuffer
	{
		SHARED_ONLY(CBuffer);

		AutoReleasePtr<ID3D11Buffer> m_buffer;
		UINT m_bufferSize;

	private:
		CBuffer(ID3D11Device* device, UINT sizeInBytes);

		void FillBufferDesc(D3D11_BUFFER_DESC& bufferDesc);
		void CreateCBuffer(ID3D11Device* device, UINT sizeInBytes);

	public:
		static CBuffer::P Create(ID3D11Device* device, UINT sizeInBytes);

		bool WriteBuffer(ID3D11DeviceContext* deviceContext, void* dataptr);
		ID3D11Buffer* getBuffer();
		UINT getBufferSize();
	};

	ID3DBlob* LoadShaderCode(const WCHAR* filename);

	enum ShaderInputLayoutType
	{
		SIL_POSITION = 0x1,
		SIL_COLOR = 0x2,
		SIL_TEXCOORD = 0x4,
		SIL_NORMAL = 0x8,
		SIL_NORMALMAP = 0x10
	};

	UINT VertexSizeInBytes(UINT shaderInputLayoutType);
	UINT VertexSizeInFloats(UINT shaderInputLayoutType);
	UINT PositionOffset(UINT shaderInputLayoutType);
	UINT ColorOffset(UINT shaderInputLayoutType);
	UINT TexcoordOffset(UINT shaderInputLayoutType);
	UINT NormalOffset(UINT shaderInputLayoutType);
	UINT TangentOffset(UINT shaderInputLayoutType);
	UINT BinormalOffset(UINT shaderInputLayoutType);

	class VertexShader
	{
		SHARED_ONLY(VertexShader);
		AutoReleasePtr<ID3D11VertexShader> m_shader;
		AutoReleasePtr<ID3D11InputLayout> m_inputLayout;
		UINT m_inputLayoutType;

	private:
		VertexShader(ID3D11Device* device, const WCHAR* shaderFileName, UINT inputLayoutType);

		int FillInputLayoutDesc(D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[]);
		void CreateVertexShader(ID3D11Device* device, ID3DBlob* shaderBuffer);
		void CreateInputLayout(ID3D11Device* device, ID3DBlob* shaderBuffer);

		void LoadVertexShader(ID3D11Device* device, const WCHAR* shaderFileName, UINT inputLayoutType);

	public:
		static VertexShader::P Create(ID3D11Device* device, const WCHAR* shaderFileName, UINT inputLayoutType);

		void SetShaderToRender(ID3D11DeviceContext* deviceContext);
		static void SetCBuffer(ID3D11DeviceContext* deviceContext, CBuffer::P buffer, UINT index = 0);
	};

	class PixelShader
	{		
		SHARED_ONLY(PixelShader);
		AutoReleasePtr<ID3D11PixelShader> m_shader;

	private:
		PixelShader(ID3D11Device* device, const WCHAR* shaderFileName);

		void LoadPixelShader(ID3D11Device* device, const WCHAR* shaderFileName);

	public:
		static PixelShader::P Create(ID3D11Device* device, const WCHAR* shaderFileName);

		void SetShaderToRender(ID3D11DeviceContext* deviceContext);

		static void SetCBuffer(ID3D11DeviceContext* deviceContext, CBuffer::P buffer, UINT index = 0);
	};
}