#include "cvt_modelmanager.h"
#include <fstream>

namespace cvt
{
	std::string WSTR2ASTR(const WCHAR* str)
	{
		std::string s;
		for (int i = 0; str[i]; i++)
			s += (char)str[i];
		return s;
	}
	std::wstring GetFileNameWithExtension(std::string fullPath)
	{
		std::wstring fileName;
		size_t minindex = fullPath.find_last_of('\\');
		size_t maxindex = fullPath.length();
		if (minindex < 0)
			minindex = 0;
		else
			minindex++;
		for (size_t i = minindex; i < maxindex; i++)
			fileName += fullPath[i];
		return fileName;
	}
	std::wstring GetFileNameWithoutExtension(std::wstring fullPath)
	{
		std::wstring fileName;
		size_t minindex = fullPath.find_last_of('\\');
		size_t maxindex = fullPath.find_last_of('.');
		if (minindex < 0)
			minindex = 0;
		else
			minindex++;
		for (size_t i = minindex; i < maxindex; i++)
			fileName += fullPath[i];
		return fileName;
	}
	std::wstring GetFolderName(std::wstring filename)
	{
		std::wstring folder;
		size_t maxindex = filename.find_last_of('\\') + 1;
		for (size_t i = 0; i < maxindex; i++)
			folder += filename[i];
		return folder;
	}
	std::wstring GetExtension(std::wstring fullPath)
	{
		std::wstring extension;
		size_t minindex = fullPath.find_last_of('.');
		size_t maxindex = fullPath.length();
		if (minindex < 0)
			return extension;
		for (size_t i = minindex; i < maxindex; i++)
			extension += fullPath[i];
		return extension;
	}

	void ModelManager::StoreData(const aiScene *scene)
	{
		std::vector<std::wstring> textureNames(scene->mNumMaterials);
		std::vector<std::wstring> normalmapNames(scene->mNumMaterials);
		for (UINT m = 0; m < scene->mNumMaterials; m++)
		{
			aiString path;
			if (scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				std::string fileName = path.data;
				textureNames[m] = GetFileNameWithExtension(fileName);
			}
			else
				textureNames[m] = std::wstring();

			if (scene->mMaterials[m]->GetTexture(aiTextureType_NORMALS, 0, &path) == AI_SUCCESS)
			{
				std::string fileName = path.data;
				normalmapNames[m] = GetFileNameWithExtension(fileName);
			}
			else
				normalmapNames[m] = std::wstring();
		}

		m_model.resize(scene->mNumMeshes);
		for (UINT m = 0; m < scene->mNumMeshes; m++)
		{
			auto mesh = scene->mMeshes[m];
			auto& model = m_model[m];
			model.vertices.resize(mesh->mNumVertices);
			for (UINT v = 0; v < mesh->mNumVertices; v++)
			{
				auto& vertex = model.vertices[v];
				if (mesh->HasPositions())
				{
					vertex.position.x = mesh->mVertices[v].x;
					vertex.position.y = mesh->mVertices[v].y;
					vertex.position.z = -mesh->mVertices[v].z;
					m_srcShaderInputType |= (UINT)gfx::ShaderInputLayoutType::POSITION;
				}
				else
				{
					vertex.position.x = 0.0f;
					vertex.position.y = 0.0f;
					vertex.position.z = 0.0f;
				}
				if (mesh->HasVertexColors(0))
				{
					vertex.color.x = mesh->mColors[0][v].r;
					vertex.color.y = mesh->mColors[0][v].g;
					vertex.color.z = mesh->mColors[0][v].b;
					vertex.color.w = mesh->mColors[0][v].a;
					//m_srcShaderInputType |= (UINT)gfx::ShaderInputLayoutType::COLOR;
				}
				else
				{
					vertex.color.x = 1.0f;
					vertex.color.y = 1.0f;
					vertex.color.z = 1.0f;
					vertex.color.w = 1.0f;
				}
				if (mesh->HasTextureCoords(0))
				{
					vertex.texcoord.x = mesh->mTextureCoords[0][v].x;
					vertex.texcoord.y = 1.0f - mesh->mTextureCoords[0][v].y;
					m_srcShaderInputType |= (UINT)gfx::ShaderInputLayoutType::TEXCOORD;
				}
				else
				{
					vertex.texcoord.x = 0.0f;
					vertex.texcoord.y = 0.0f;
				}
				if (mesh->HasNormals())
				{
					vertex.normal.x = mesh->mNormals[v].x;
					vertex.normal.y = mesh->mNormals[v].y;
					vertex.normal.z = -mesh->mNormals[v].z;
					m_srcShaderInputType |= (UINT)gfx::ShaderInputLayoutType::NORMAL;
				}
				else
				{
					vertex.normal.x = 0.0f;
					vertex.normal.y = 1.0f;
					vertex.normal.z = 0.0f;
				}
				if (mesh->HasTangentsAndBitangents())
				{
					vertex.tangent.x = mesh->mTangents[v].x;
					vertex.tangent.y = mesh->mTangents[v].y;
					vertex.tangent.z = -mesh->mTangents[v].z;
					vertex.binormal.x = -mesh->mBitangents[v].x;
					vertex.binormal.y = -mesh->mBitangents[v].y;
					vertex.binormal.z = mesh->mBitangents[v].z;
					m_srcShaderInputType |= (UINT)gfx::ShaderInputLayoutType::NORMALMAP;
				}
				else
				{
					vertex.tangent.x = 1.0f;
					vertex.tangent.y = 0.0f;
					vertex.tangent.z = 0.0f;
					vertex.binormal.x = 0.0f;
					vertex.binormal.y = 0.0f;
					vertex.binormal.z = 1.0f;
				}
			}
			for (UINT f = 0; f < mesh->mNumFaces; f++)
			{
				for (UINT i = 2; i < mesh->mFaces[f].mNumIndices; i++)
				{
					model.indices.push_back(mesh->mFaces[f].mIndices[0]);
					model.indices.push_back(mesh->mFaces[f].mIndices[i]);
					model.indices.push_back(mesh->mFaces[f].mIndices[i - 1]);
				}
			}
			model.textureName = textureNames[mesh->mMaterialIndex];
			model.normalmapName = normalmapNames[mesh->mMaterialIndex];
		}
	}

	bool ModelManager::LoadOMD(const WCHAR *filename)
	{
		std::ifstream infile;
		OMD_Header header;
		OMD_ModelData mdlData;

		infile.open(filename, std::ios::in | std::ios::binary);
		infile.read((char*)&header, sizeof(header));
		m_srcShaderInputType = header.shaderInputLayout;
		m_model.resize(header.modelCount);
		for (auto& m : m_model)
		{
			infile.read((char*)&mdlData, sizeof(mdlData));
			m.vertices.resize(mdlData.vertexCount);
			m.indices.resize(mdlData.indexCount);
			m.textureName.resize(mdlData.textureNameLen);
			m.normalmapName.resize(mdlData.normalmapNameLen);
			for (auto& v : m.vertices)
			{
				if (header.shaderInputLayout & (UINT)gfx::ShaderInputLayoutType::POSITION)
					infile.read((char*)&v.position, sizeof(v.position));
				if (header.shaderInputLayout & ((UINT)gfx::ShaderInputLayoutType::TEXCOORD | (UINT)gfx::ShaderInputLayoutType::NORMALMAP))
					infile.read((char*)&v.texcoord, sizeof(v.texcoord));
				if (header.shaderInputLayout & (UINT)gfx::ShaderInputLayoutType::NORMAL)
					infile.read((char*)&v.normal, sizeof(v.normal));
				if (header.shaderInputLayout & (UINT)gfx::ShaderInputLayoutType::NORMALMAP)
				{
					infile.read((char*)&v.tangent, sizeof(v.tangent));
					infile.read((char*)&v.binormal, sizeof(v.binormal));
				}
			}
			infile.read((char*)m.indices.data(), m.indices.size() * sizeof(UINT));
			infile.read((char*)m.textureName.data(), m.textureName.length() * sizeof(WCHAR));
			infile.read((char*)m.normalmapName.data(), m.normalmapName.length() * sizeof(WCHAR));
		}

		infile.close();
		return true;
	}

	bool ModelManager::LoadModelData(const WCHAR* filename)
	{
		Clear();
		if (GetExtension(filename) == L".omd")
		{
			if (!LoadOMD(filename))
				return false;
		}
		else
		{
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(WSTR2ASTR(filename).c_str(),
				aiProcess_CalcTangentSpace |
				aiProcess_JoinIdenticalVertices |
				aiProcess_Triangulate |
				aiProcess_SortByPType);
			if (scene == NULL)
			{
				auto error = importer.GetErrorString();
				std::wstring errormsg;
				for (int i = 0; error[i]; i++)
					errormsg += (WCHAR)error[i];
				throw hcs::Exception(L"Import failed", errormsg.c_str());
			}
			StoreData(scene);
		}
		m_sourceFile = GetFileNameWithoutExtension(filename);
		m_sourceFolder = GetFolderName(filename);
		return true;
	}

	void ModelManager::Clear()
	{
		m_sourceFolder.clear();
		m_sourceFile.clear();
		m_model.clear();
		m_srcShaderInputType = 0;
	}

	UINT ModelManager::getModelPartCount()
	{
		return (UINT)m_model.size();
	}

	UINT ModelManager::getShaderInputType()
	{
		return m_srcShaderInputType;
	}

	std::wstring ModelManager::getOutFilename()
	{
		return m_sourceFolder + m_sourceFile + L".omd";
	}

	gfx::Entity::P ModelManager::CreateEntity(gfx::Graphics::P graphics)
	{
		ID3D11Device* device = graphics->getDevice();
		UINT shaderInputType =
			(UINT)gfx::ShaderInputLayoutType::POSITION |
			(UINT)gfx::ShaderInputLayoutType::COLOR |
			(UINT)gfx::ShaderInputLayoutType::TEXCOORD |
			(UINT)gfx::ShaderInputLayoutType::NORMAL |
			(UINT)gfx::ShaderInputLayoutType::NORMALMAP;

		std::vector<gfx::Model::P> models;
		std::vector<gfx::Texture::P> textures, normalmaps;
		models.resize(m_model.size());
		textures.resize(m_model.size());
		normalmaps.resize(m_model.size());
		gfx::Texture::P white = gfx::Texture::Create2D(device, L"Media/white.png");
		gfx::Texture::P normal = gfx::Texture::Create2D(device, L"Media/normal.png");
		for (int i = 0; i < m_model.size(); i++)
		{
			models[i] = gfx::Model::Create(device, m_model[i].vertices.data(), (UINT)m_model[i].vertices.size(),
				shaderInputType, m_model[i].indices.data(), (UINT)m_model[i].indices.size());
			if (m_model[i].textureName[0])
				textures[i] = gfx::Texture::Create2D(device, (m_sourceFolder + m_model[i].textureName).c_str());
			else
				textures[i] = white;
			if (m_model[i].normalmapName[0])
				normalmaps[i] = gfx::Texture::Create2D(device, (m_sourceFolder + m_model[i].normalmapName).c_str());
			else
				normalmaps[i] = normal;
		}
		return gfx::Entity::Create(models.data(), textures.data(),normalmaps.data(), (UINT)m_model.size());
	}

	std::wstring& ModelManager::getTextureName(UINT index)
	{
		return m_model[index].textureName;
	}

	std::wstring& ModelManager::getNormalmapeName(UINT index)
	{
		return m_model[index].normalmapName;
	}

	void ModelManager::Export(const WCHAR *filename, UINT shaderInputLayout)
	{
		std::ofstream outfile;
		OMD_Header header;
		OMD_ModelData mdlData;

		header.shaderInputLayout = shaderInputLayout;
		header.modelCount = (UINT)m_model.size();

		outfile.open(filename ? filename : getOutFilename(), std::ios::out | std::ios::binary);
		outfile.write((char*)&header, sizeof(header));
		for (auto& m : m_model)
		{
			mdlData.vertexCount = (UINT)m.vertices.size();
			mdlData.indexCount = (UINT)m.indices.size();
			mdlData.textureNameLen = (UINT)m.textureName.length();
			mdlData.normalmapNameLen = (UINT)m.normalmapName.length();
			outfile.write((char*)&mdlData, sizeof(mdlData));
			for (auto& v : m.vertices)
			{
				if (header.shaderInputLayout & (UINT)gfx::ShaderInputLayoutType::POSITION)
					outfile.write((char*)&v.position, sizeof(v.position));
				if (header.shaderInputLayout & ((UINT)gfx::ShaderInputLayoutType::TEXCOORD | (UINT)gfx::ShaderInputLayoutType::NORMALMAP))
					outfile.write((char*)&v.texcoord, sizeof(v.texcoord));
				if (header.shaderInputLayout & (UINT)gfx::ShaderInputLayoutType::NORMAL)
					outfile.write((char*)&v.normal, sizeof(v.normal));
				if (header.shaderInputLayout & (UINT)gfx::ShaderInputLayoutType::NORMALMAP)
				{
					outfile.write((char*)&v.tangent, sizeof(v.tangent));
					outfile.write((char*)&v.binormal, sizeof(v.binormal));
				}
			}
			outfile.write((char*)m.indices.data(), m.indices.size() * sizeof(UINT));
			outfile.write((char*)m.textureName.data(), m.textureName.length() * sizeof(WCHAR));
			outfile.write((char*)m.normalmapName.data(), m.normalmapName.length() * sizeof(WCHAR));
		}

		outfile.close();
	}

}