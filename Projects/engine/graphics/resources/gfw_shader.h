#pragma once

#include "graphics/gfw_graphics.h"

namespace gfw
{
	class CBuffer :public Resource
	{
		AutoReleasePtr<ID3D11Buffer> m_buffer;
		UINT m_bufferSize;

	private:
		void FillBufferDesc(D3D11_BUFFER_DESC& bufferDesc);

	public:
		CBuffer();
		CBuffer(ID3D11Device* device, UINT sizeInBytes);
		static std::shared_ptr<CBuffer> Create();
		static std::shared_ptr<CBuffer> Create(ID3D11Device* device, UINT sizeInBytes);

		void CreateCBuffer(ID3D11Device* device, UINT sizeInBytes);
		bool WriteBuffer(ID3D11DeviceContext* deviceContext, void* dataptr);
		virtual void Release() override;
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

	class VertexShader :public Resource
	{
		AutoReleasePtr<ID3D11VertexShader> m_shader;
		AutoReleasePtr<ID3D11InputLayout> m_inputLayout;
		UINT m_inputLayoutType;

	private:
		int FillInputLayoutDesc(D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[]);
		void CreateVertexShader(ID3D11Device* device, ID3DBlob* shaderBuffer);
		void CreateInputLayout(ID3D11Device* device, ID3DBlob* shaderBuffer);

	public:
		VertexShader();
		VertexShader(ID3D11Device* device, const WCHAR* shaderFileName, UINT inputLayoutType);
		static std::shared_ptr<VertexShader> Create();
		static std::shared_ptr<VertexShader> Create(ID3D11Device* device, const WCHAR* shaderFileName, UINT inputLayoutType);

		void LoadVertexShader(ID3D11Device* device, const WCHAR* shaderFileName, UINT inputLayoutType);
		virtual void Release() override;

		void SetShaderToRender(ID3D11DeviceContext* deviceContext);
		static void SetCBuffer(ID3D11DeviceContext* deviceContext, CBuffer& buffer, UINT index = 0);
	};

	class PixelShader :public Resource
	{		
		AutoReleasePtr<ID3D11PixelShader> m_shader;

	public:
		PixelShader();
		PixelShader(ID3D11Device* device, const WCHAR* shaderFileName);
		static std::shared_ptr<PixelShader> Create();
		static std::shared_ptr<PixelShader> Create(ID3D11Device* device, const WCHAR* shaderFileName);

		void LoadPixelShader(ID3D11Device* device, const WCHAR* shaderFileName);
		virtual void Release() override;

		void SetShaderToRender(ID3D11DeviceContext* deviceContext);

		static void SetCBuffer(ID3D11DeviceContext* deviceContext, CBuffer& buffer, UINT index = 0);
	};
}