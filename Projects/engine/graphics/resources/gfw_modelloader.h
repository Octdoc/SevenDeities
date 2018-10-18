#pragma once
#pragma once

#include "graphics/gfw_graphics.h"
#include "math/mth_linalg.h"

namespace gfw
{
	struct VertexType
	{
		mth::float3 position;
		mth::float4 color;
		mth::float2 texcoord;
		mth::float3 normal;
		mth::float3 tangent;
		mth::float3 binormal;
	};

	class ModelLoader
	{
	protected:
		std::wstring m_filename;

		UINT m_vertexSizeInBytes;
		UINT m_shaderInputLayout;

		std::vector<float> m_vertices;
		std::vector<UINT> m_indices;

	private:
		void Create(VertexType vertices[], UINT vertexCount, UINT indices[], UINT indexCount, UINT shaderInputLayout);

	public:
		ModelLoader();
		ModelLoader(const WCHAR* filename);

		UINT getVertexSizeInBytes();
		UINT getVertexSizeInFloats();
		UINT getShaderInputLayout();
		UINT getVertexCount();
		UINT getIndexCount();
		UINT* getIndices();
		float* getVertices();
		std::wstring& getFileName();

		void Clear();

		void LoadModel(const WCHAR* filename);
		void CreateCube(mth::float3 position, mth::float3 size, UINT shaderInputLayout);
		void CreateCube(mth::float3 position, mth::float3 size, UINT shaderInputLayout, float r, float g, float b, float a = 1.0f);
		void CreateFullScreenQuad();
		void CreateScreenQuad(mth::float2 pos, mth::float2 size);
		void CreateQuad(mth::float2 pos, mth::float2 size, UINT shaderInputLayout);
		void CreateQuad(mth::float2 pos, mth::float2 size, mth::float2 tpos, mth::float2 tsize, UINT shaderInputLayout);
		void CreateQuad(mth::float2 pos, mth::float2 size, UINT shaderInputLayout, float r, float g, float b, float a = 1.0f);
		void CreateQuad(mth::float2 pos, mth::float2 size, mth::float2 tpos, mth::float2 tsize, UINT shaderInputLayout, float r, float g, float b, float a = 1.0f);

		void FlipInsideOut();
		void Transform(mth::float4x4 transform);
	};
}