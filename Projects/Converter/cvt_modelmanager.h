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
		UINT modelCount;
	};
	struct OMD_ModelData
	{
		UINT vertexCount;
		UINT indexCount;
	};

	struct ModelPart
	{
		std::vector<gfx::VertexType> vertices;
		std::vector<UINT> indices;
		std::wstring textureName;
		std::wstring normalmapName;
	};

	class ModelManager
	{
		std::wstring m_sourceFolder;
		std::wstring m_sourceFile;
		std::vector<ModelPart> m_model;
		UINT m_srcShaderInputType;

	private:
		void StoreData(const aiScene* scene);
		bool LoadOMD(const WCHAR* filename);

	public:
		bool LoadModelData(const WCHAR* filename);
		void Clear();

		UINT getModelPartCount();
		UINT getShaderInputType();
		std::wstring getOutFilename();
		gfx::Entity::P CreateEntity(gfx::Graphics::P graphics);
		std::wstring& getTextureName(UINT index);
		std::wstring& getNormalmapeName(UINT index);

		void Export(const WCHAR *filename, UINT shaderInputLayout);
	};
}