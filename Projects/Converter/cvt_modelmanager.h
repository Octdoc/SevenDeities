#pragma once

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "octdoc.h"

using namespace octdoc;

namespace cvt
{
	struct Vertex
	{
		mth::float3 position;
		mth::float2 texcoord;
		mth::float3 normal;
		mth::float3 tangent;
		mth::float3 binormal;
	};

	struct Group
	{
		std::vector<UINT> indices;
		std::wstring textureName;
		std::wstring normalmapName;
	};

	struct OMD_Header
	{
		UINT shaderInputLayout;
		UINT vertexCount;
		UINT indexCount;
	};

	class ModelManager
	{
		std::wstring m_sourceFolder;
		std::wstring m_sourceFile;

		std::vector<Vertex> m_vertices;
		std::vector<UINT> m_indices;
		std::vector<UINT> m_indexGroupSizes;
		std::vector<std::wstring> m_textureNames;
		std::vector<std::wstring> m_normalmapNames;
		UINT m_shaderInputType;

	private:
		void StoreData(const aiScene* scene);

	public:
		bool LoadModelData(const WCHAR* filename);
		void Clear();

		UINT getShaderInputType();
		std::wstring getOutFilename();
		gfx::Entity::P CreateEntity(gfx::Graphics::P graphics);

		void Export(const WCHAR *filename, UINT shaderInputLayout);
	};
}