#include "cvt_modelmanager.h"
#include <fstream>
#include <iostream>

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

	void ModelManager::StoreData(const aiScene* scene)
	{
		UINT vertexCount = 0, indexCount = 0, offset = 0;
		std::vector<std::wstring> textureNames, normalmapNames;
		std::vector<Group> groups;
		aiMesh* mesh;
		Group* group;
		aiString path;

		textureNames.clear();
		normalmapNames.clear();
		groups.resize(scene->mNumMeshes);
		textureNames.resize(scene->mNumMaterials);
		normalmapNames.resize(scene->mNumMaterials);

		for (unsigned int m = 0; m < scene->mNumMeshes; m++)
			vertexCount += scene->mMeshes[m]->mNumVertices;
		m_vertices.resize(vertexCount);

		for (unsigned int m = 0; m < scene->mNumMaterials; m++)
		{
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

		UINT counter = 0;
		for (unsigned int m = 0; m < scene->mNumMeshes; m++)
		{
			mesh = scene->mMeshes[m];
			group = &groups[m];
			for (UINT v = 0; v < mesh->mNumVertices; v++)
			{
				if (mesh->HasPositions())
				{
					m_vertices[counter].position.x = mesh->mVertices[v].x;
					m_vertices[counter].position.y = mesh->mVertices[v].y;
					m_vertices[counter].position.z = -mesh->mVertices[v].z;
				}
				else
				{
					m_vertices[counter].position.x = 0.0f;
					m_vertices[counter].position.y = 0.0f;
					m_vertices[counter].position.z = 0.0f;
				}
				/*if (mesh->HasVertexColors(0))
				{
					m_vertices[counter].color.x = mesh->mColors[0]->r;
					m_vertices[counter].color.y = mesh->mColors[0]->g;
					m_vertices[counter].color.z = mesh->mColors[0]->b;
					m_vertices[counter].color.w = mesh->mColors[0]->a;
				}
				else
				{
					m_vertices[counter].color.x = 1.0f;
					m_vertices[counter].color.y = 1.0f;
					m_vertices[counter].color.z = 1.0f;
					m_vertices[counter].color.w = 1.0f;
				}*/
				if (mesh->HasTextureCoords(0))
				{
					m_vertices[counter].texcoord.x = mesh->mTextureCoords[0][v].x;
					m_vertices[counter].texcoord.y = 1.0f - mesh->mTextureCoords[0][v].y;
				}
				else
				{
					m_vertices[counter].texcoord.x = 0.0f;
					m_vertices[counter].texcoord.y = 0.0f;
				}
				if (mesh->HasNormals())
				{
					m_vertices[counter].normal.x = mesh->mNormals[v].x;
					m_vertices[counter].normal.y = mesh->mNormals[v].y;
					m_vertices[counter].normal.z = -mesh->mNormals[v].z;
				}
				else
				{
					m_vertices[counter].normal.x = 0.0f;
					m_vertices[counter].normal.y = 1.0f;
					m_vertices[counter].normal.z = 0.0f;
				}
				if (mesh->HasTangentsAndBitangents())
				{
					m_vertices[counter].tangent.x = mesh->mTangents[v].x;
					m_vertices[counter].tangent.y = mesh->mTangents[v].y;
					m_vertices[counter].tangent.z = -mesh->mTangents[v].z;
					m_vertices[counter].binormal.x = -mesh->mBitangents[v].x;
					m_vertices[counter].binormal.y = -mesh->mBitangents[v].y;
					m_vertices[counter].binormal.z = mesh->mBitangents[v].z;
				}
				else
				{
					m_vertices[counter].tangent.x = 1.0f;
					m_vertices[counter].tangent.y = 0.0f;
					m_vertices[counter].tangent.z = 0.0f;
					m_vertices[counter].binormal.x = 0.0f;
					m_vertices[counter].binormal.y = 0.0f;
					m_vertices[counter].binormal.z = 1.0f;
				}
				counter++;
			}
			for (UINT f = 0; f < mesh->mNumFaces; f++)
			{
				for (UINT i = 2; i < mesh->mFaces[f].mNumIndices; i++)
				{
					group->indices.push_back(mesh->mFaces[f].mIndices[0] + offset);
					group->indices.push_back(mesh->mFaces[f].mIndices[i] + offset);
					group->indices.push_back(mesh->mFaces[f].mIndices[i - 1] + offset);
				}
			}
			group->textureName = textureNames[mesh->mMaterialIndex];
			group->normalmapName = normalmapNames[mesh->mMaterialIndex];
			indexCount += (UINT)group->indices.size();
			offset += mesh->mNumVertices;
		}

		bool duplicate;
		for (int g = 0; g < groups.size(); g++)
		{
			group = &groups[g];
			duplicate = false;
			for (int i = 0; i < m_textureNames.size(); i++)
				if (m_textureNames[i] == group->textureName &&
					m_normalmapNames[i] == group->normalmapName)
				{
					duplicate = true;
					break;
				}
			if (!duplicate)
			{
				m_textureNames.push_back(group->textureName);
				m_normalmapNames.push_back(group->normalmapName);
			}
		}

		m_indices.reserve(indexCount);
		m_indexGroupSizes.resize(m_textureNames.size());
		for (int i = 0; i < m_indexGroupSizes.size(); i++)
		{
			m_indexGroupSizes[i] = 0;
			for (int g = 0; g < groups.size(); g++)
			{
				group = &groups[g];
				if (m_textureNames[i] == group->textureName &&
					m_normalmapNames[i] == group->normalmapName)
				{
					m_indexGroupSizes[i] += (UINT)group->indices.size();
					m_indices.insert(m_indices.end(), group->indices.begin(), group->indices.end());
				}
			}
		}
	}

	bool ModelManager::LoadModelData(const WCHAR* filename)
	{
		Clear();
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(WSTR2ASTR(filename).c_str(),
			aiProcess_CalcTangentSpace |
			aiProcess_JoinIdenticalVertices |
			aiProcess_Triangulate |
			aiProcess_SortByPType);
		if (scene == NULL)
		{
			std::cout << "Import failed" << std::endl;
			std::cout << importer.GetErrorString() << std::endl;
			return false;
		}

		StoreData(scene);
		m_sourceFile = GetFileNameWithoutExtension(filename);
		m_sourceFolder = GetFolderName(filename);
		std::wcout << L"Model loaded" << std::endl
			<< L"Vertex count: " << m_vertices.size() << std::endl
			<< L"Index count: " << m_indices.size() << std::endl
			<< L"Group count: " << m_indexGroupSizes.size() << std::endl;
		return true;
	}

	void ModelManager::Clear()
	{
		m_sourceFolder.clear();
		m_sourceFile.clear();
		m_vertices.clear();
		m_indices.clear();
		m_indexGroupSizes.clear();
		m_textureNames.clear();
		m_normalmapNames.clear();
	}

	gfw::Entity::P ModelManager::CreateEntity(gfw::Graphics::P graphics)
	{
		ID3D11Device* device = graphics->getDevice();
		UINT shaderInputType = gfw::SIL_POSITION | gfw::SIL_TEXCOORD | gfw::SIL_NORMAL | gfw::SIL_NORMALMAP;
		
		std::vector<std::shared_ptr<gfw::Texture>> textures, normalmaps;
		textures.resize(m_textureNames.size());
		normalmaps.resize(m_textureNames.size());
		std::shared_ptr<gfw::Texture> white = gfw::Texture::Create2D(device, L"Media/white.png");
		std::shared_ptr<gfw::Texture> normal = gfw::Texture::Create2D(device, L"Media/normal.png");
		for (int i = 0; i < m_textureNames.size(); i++)
		{
			if (m_textureNames[i][0])
				textures[i] = gfw::Texture::Create2D(device, (m_sourceFolder + m_textureNames[i]).c_str());
			else
				textures[i] = white;
			if (m_normalmapNames[i][0])
				normalmaps[i] = gfw::Texture::Create2D(device, (m_sourceFolder + m_normalmapNames[i]).c_str());
			else
				normalmaps[i] = normal;
		}
		std::shared_ptr<gfw::Entity> entity = gfw::Entity::Create(
			gfw::Model::Create(device, m_vertices.data(), (UINT)m_vertices.size(),
			shaderInputType, m_indices.data(), (UINT)m_indices.size()),
			textures.data(), normalmaps.data());
		std::wcout << L"Entity created" << std::endl;
		return entity;
	}

	void ModelManager::Export(UINT shaderInputLayout)
	{
		std::ofstream outfile;
		OMD_Header header;
		std::wstring filename;

		filename = m_sourceFolder + m_sourceFile + L".omd";
		std::wcout << L"Exporting file: " << filename << std::endl;

		header.shaderInputLayout = gfw::SIL_POSITION | gfw::SIL_TEXCOORD | gfw::SIL_NORMAL | gfw::SIL_NORMALMAP;
		header.vertexCount = (UINT)m_vertices.size();
		header.indexCount = (UINT)m_indices.size();

		outfile.open(filename, std::ios::out | std::ios::binary);
		outfile.write((char*)&header, sizeof(header));
		outfile.write((char*)m_vertices.data(), m_vertices.size() * sizeof(gfw::VertexType));
		outfile.write((char*)m_indices.data(), m_indices.size() * sizeof(UINT));

		outfile.close();

		std::wcout << L"File exported" << std::endl;
	}

}