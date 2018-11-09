#include "gfx_modelloader.h"
#include "gfx_shader.h"
#include <fstream>

namespace octdoc
{
	namespace gfx
	{
		void ModelLoader::Create(VertexType vertices[], UINT vertexCount, UINT indices[], UINT indexCount, UINT shaderInputLayout)
		{
			int counter = 0;
			m_shaderInputLayout = shaderInputLayout;
			m_vertexSizeInBytes = VertexSizeInBytes(m_shaderInputLayout);
			m_vertices.resize(vertexCount*getVertexSizeInFloats());
			m_indices.resize(indexCount);
			for (UINT v = 0; v < vertexCount; v++)
			{
				if (m_shaderInputLayout & SIL_POSITION)
				{
					m_vertices[counter++] = vertices[v].position.x;
					m_vertices[counter++] = vertices[v].position.y;
					m_vertices[counter++] = vertices[v].position.z;
				}
				if (m_shaderInputLayout & SIL_COLOR)
				{
					m_vertices[counter++] = vertices[v].color.x;
					m_vertices[counter++] = vertices[v].color.y;
					m_vertices[counter++] = vertices[v].color.z;
					m_vertices[counter++] = vertices[v].color.w;
				}
				if (m_shaderInputLayout & (SIL_TEXCOORD | SIL_NORMALMAP))
				{
					m_vertices[counter++] = vertices[v].texcoord.x;
					m_vertices[counter++] = vertices[v].texcoord.y;
				}
				if (m_shaderInputLayout & SIL_NORMAL)
				{
					m_vertices[counter++] = vertices[v].normal.x;
					m_vertices[counter++] = vertices[v].normal.y;
					m_vertices[counter++] = vertices[v].normal.z;
				}
				if (m_shaderInputLayout & SIL_NORMALMAP)
				{
					m_vertices[counter++] = vertices[v].tangent.x;
					m_vertices[counter++] = vertices[v].tangent.y;
					m_vertices[counter++] = vertices[v].tangent.z;
					m_vertices[counter++] = vertices[v].binormal.x;
					m_vertices[counter++] = vertices[v].binormal.y;
					m_vertices[counter++] = vertices[v].binormal.z;
				}
			}
			for (UINT i = 0; i < indexCount; i++)
				m_indices[i] = indices[i];
		}

		ModelLoader::ModelLoader() :m_vertexSizeInBytes(0), m_shaderInputLayout(0) {}
		ModelLoader::ModelLoader(const WCHAR* filename) : m_vertexSizeInBytes(0), m_shaderInputLayout(0)
		{
			LoadModel(filename);
		}
		UINT ModelLoader::getVertexSizeInBytes()
		{
			return m_vertexSizeInBytes;
		}
		UINT ModelLoader::getVertexSizeInFloats()
		{
			return m_vertexSizeInBytes / sizeof(float);
		}
		UINT ModelLoader::getShaderInputLayout()
		{
			return m_shaderInputLayout;
		}
		UINT ModelLoader::getVertexCount()
		{
			return (UINT)m_vertices.size() * sizeof(float) / m_vertexSizeInBytes;
		}
		UINT ModelLoader::getIndexCount()
		{
			return (UINT)m_indices.size();
		}
		UINT* ModelLoader::getIndices()
		{
			return m_indices.data();
		}
		float* ModelLoader::getVertices()
		{
			return m_vertices.data();
		}
		std::wstring& ModelLoader::getFileName()
		{
			return m_filename;
		}
		void ModelLoader::Clear()
		{
			m_filename.clear();
			m_vertexSizeInBytes = 0;
			m_shaderInputLayout = 0;
			m_vertices.clear();
			m_indices.clear();
		}
		void ModelLoader::LoadModel(const WCHAR* filename)
		{
			std::ifstream infile;
			UINT vertexCount, indexCount;

			infile.open(filename, std::ios::in | std::ios::binary);
			if (!infile.good())
				throw hcs::Exception::FileLoadingException(filename);
			m_filename = filename;
			infile.read((char*)&m_shaderInputLayout, sizeof(UINT));
			infile.read((char*)&vertexCount, sizeof(UINT));
			infile.read((char*)&indexCount, sizeof(UINT));
			m_vertexSizeInBytes = VertexSizeInBytes(m_shaderInputLayout);
			m_vertices.resize(vertexCount*m_vertexSizeInBytes / sizeof(float));
			m_indices.resize(indexCount);
			infile.read((char*)m_vertices.data(), m_vertices.size() * sizeof(float));
			infile.read((char*)m_indices.data(), m_indices.size() * sizeof(UINT));
			infile.close();
		}

		void ModelLoader::CreateCube(mth::float3 position, mth::float3 size, UINT shaderInputLayout)
		{
			CreateCube(position, size, shaderInputLayout, 1.0f, 1.0f, 1.0f, 1.0f);
		}

		void ModelLoader::CreateCube(mth::float3 p, mth::float3 s, UINT shaderInputLayout, float r, float g, float b, float a)
		{
			VertexType vertices[] = {
				{{p.x + s.x, p.y, p.z + s.z},{r,g,b,a}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}},
				{{p.x + s.x, p.y, p.z},{r,g,b,a}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}},
				{{p.x, p.y, p.z},{r,g,b,a}, {1.0f, 0.0f}, {0.0f, -1.0f,0.0f}, {0.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}},
				{{p.x, p.y, p.z + s.z},{r,g,b,a}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}},
				{{p.x + s.x, p.y + s.y, p.z + s.z},{r,g,b,a},{ 0.0f, 1.0f} ,{ 0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
				{{p.x, p.y + s.y, p.z + s.z},{r,g,b,a}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
				{{p.x, p.y + s.y, p.z},{r,g,b,a},{ 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
				{{p.x + s.x, p.y + s[1], p.z},{r,g,b,a}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
				{{p.x + s.x, p.y, p.z + s.z},{r,g,b,a}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f, 0.0f, -1.0f}},
				{{p.x + s.x, p.y + s.y, p.z + s.z},{r,g,b,a}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f},{0.0f, 1.0f, 0.0f},{ 0.0f, 0.0f, -1.0f}},
				{{p.x + s.x, p.y + s.y, p.z},{r,g,b,a},{ 1.0f, 0.0f},{ 1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
				{{p.x + s.x, p.y, p.z},{r,g,b,a},{ 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f, 0.0f, -1.0f}},
				{{p.x + s.x, p.y, p.z},{r,g,b,a},{ 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f},{ 0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
				{{p.x + s.x, p.y + s.y, p.y},{r,g,b,a},{ 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
				{{p.x, p.y + s.y, p.z},{r,g,b,a},{ 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
				{{p.x, p.y, p.z},{r,g,b,a},{ 0.0f, 0.0f},{ 0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
				{{p.x, p.y, p.z},{r,g,b,a},{ 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f},{ 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
				{{p.x, p.y + s.y, p.z},{r,g,b,a}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f},{ 0.0f, 1.0f, 0.0f},{ 0.0f, 0.0f, 1.0f}},
				{{p.x, p.y + s.y, p.z + s.z},{r,g,b,a},{ 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},{ 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
				{{p.x, p.y, p.z + s.z},{r,g,b,a},{0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},{ 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
				{{p.x + s.x, p.y + s.y, p.z + s.z},{r,g,b,a},{ 0.0f, 1.0f},{ 0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
				{{p.x + s.x, p.y, p.z + s.z},{r,g,b,a},{ 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f},{ -1.0f, 0.0f, 0.0f}},
				{{p.x, p.y, p.z + s.z},{r,g,b,a}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f},{ -1.0f, 0.0f, 0.0f}},
				{{p.x, p.y + s.y, p.z + s.z},{r,g,b,a},{ 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}}
			};
			UINT indices[] = {
				0, 2, 1, 0, 3, 2, 4, 6, 5, 4, 7, 6, 8, 10, 9, 8, 11, 10, 12, 14, 13, 12, 15, 14, 16, 18, 17, 16, 19, 18, 20, 22, 21, 20, 23, 22
			};
			Create(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]), shaderInputLayout);
		}

		void ModelLoader::CreateFullScreenQuad()
		{
			CreateQuad({ -1.0f, -1.0f }, { 2.0f, 2.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, SIL_POSITION | SIL_TEXCOORD, 1.0f, 1.0f, 1.0f, 1.0f);
			Transform(mth::float4x4::Translation(0.0f, 0.0f, 1.0f)*mth::float4x4::RotationX(-mth::pi*0.5f));
		}

		void ModelLoader::CreateScreenQuad(mth::float2 pos, mth::float2 size)
		{
			CreateQuad(pos, size, { 0.0f, 0.0f }, { 1.0f, 1.0f }, SIL_POSITION | SIL_TEXCOORD, 1.0f, 1.0f, 1.0f, 1.0f);
			Transform(mth::float4x4::Translation(0.0f, 0.0f, 1.0f)*mth::float4x4::RotationX(-mth::pi*0.5f));
		}

		void ModelLoader::CreateQuad(mth::float2 pos, mth::float2 size, UINT shaderInputLayout)
		{
			CreateQuad(pos, size, { 0.0f, 0.0f }, { 1.0f, 1.0f }, shaderInputLayout, 1.0f, 1.0f, 1.0f, 1.0f);
		}

		void ModelLoader::CreateQuad(mth::float2 pos, mth::float2 size, mth::float2 tpos, mth::float2 tsize, UINT shaderInputLayout)
		{
			CreateQuad(pos, size, tpos, tsize, shaderInputLayout, 1.0f, 1.0f, 1.0f, 1.0f);
		}

		void ModelLoader::CreateQuad(mth::float2 pos, mth::float2 size, UINT shaderInputLayout, float r, float g, float b, float a)
		{
			CreateQuad(pos, size, { 1.0f, 1.0f }, { 0.0f, 0.0f }, shaderInputLayout, r, g, b, a);
		}

		void ModelLoader::CreateQuad(mth::float2 pos, mth::float2 size, mth::float2 tpos, mth::float2 tsize, UINT shaderInputLayout, float r, float g, float b, float a)
		{
			VertexType vertices[] = {
			{ {pos.x + size.x, 0.0f, pos.y}, { r, g, b, a }, { tpos.x + tsize.x, tpos.y + tsize.y }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
			{ {pos.x + size.x, 0.0f, pos.y + size.y}, {r, g, b, a}, { tpos.x + tsize.x, tpos.y }, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
			{ {pos.x, 0.0f, pos.y + size.y}, {r, g, b, a}, { tpos.x, tpos.y }, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
			{ {pos.x, 0.0f, pos.y}, {r, g, b, a}, { tpos.x, tpos.y + tsize.y }, { 0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} }
			};
			UINT indices[] = { 0, 2, 1, 0, 3, 2 };
			Create(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]), shaderInputLayout);
		}

		void ModelLoader::FlipInsideOut()
		{
			for (int i = 0; i < m_indices.size(); i += 3)
			{
				UINT tmp = m_indices[i + 1];
				m_indices[i + 1] = m_indices[i + 2];
				m_indices[i + 2] = tmp;
			}
			UINT vertexSize = getVertexSizeInFloats();
			if (m_shaderInputLayout & SIL_NORMAL)
			{
				UINT offset = NormalOffset(m_shaderInputLayout);
				for (UINT i = 0; i < getVertexCount(); i++)
				{
					m_vertices[i*vertexSize + offset + 0] *= -1.0f;
					m_vertices[i*vertexSize + offset + 1] *= -1.0f;
					m_vertices[i*vertexSize + offset + 2] *= -1.0f;
				}
			}
			if (m_shaderInputLayout & SIL_NORMALMAP)
			{
				UINT offset1 = TangentOffset(m_shaderInputLayout);
				UINT offset2 = BinormalOffset(m_shaderInputLayout);
				for (UINT i = 0; i < getVertexCount(); i++)
				{
					m_vertices[i*vertexSize + offset1 + 0] *= -1.0f;
					m_vertices[i*vertexSize + offset1 + 1] *= -1.0f;
					m_vertices[i*vertexSize + offset1 + 2] *= -1.0f;
					m_vertices[i*vertexSize + offset2 + 0] *= -1.0f;
					m_vertices[i*vertexSize + offset2 + 1] *= -1.0f;
					m_vertices[i*vertexSize + offset2 + 2] *= -1.0f;
				}
			}
		}

		void ModelLoader::Transform(mth::float4x4 transform)
		{
			UINT vertexSize = getVertexSizeInFloats();
			mth::float4 v;
			if (m_shaderInputLayout & SIL_POSITION)
			{
				UINT offset = PositionOffset(m_shaderInputLayout);
				for (UINT i = 0; i < getVertexCount(); i++)
				{
					v.x = m_vertices[i*vertexSize + offset + 0];
					v.y = m_vertices[i*vertexSize + offset + 1];
					v.z = m_vertices[i*vertexSize + offset + 2];
					v.w = 1;
					v = transform * v;
					m_vertices[i*vertexSize + offset + 0] = v.x;
					m_vertices[i*vertexSize + offset + 1] = v.y;
					m_vertices[i*vertexSize + offset + 2] = v.z;
				}
			}
			if (m_shaderInputLayout & SIL_NORMAL)
			{
				UINT offset = NormalOffset(m_shaderInputLayout);
				for (UINT i = 0; i < getVertexCount(); i++)
				{
					v.x = m_vertices[i*vertexSize + offset + 0];
					v.y = m_vertices[i*vertexSize + offset + 1];
					v.z = m_vertices[i*vertexSize + offset + 2];
					v.w = 1;
					v = transform * v;
					m_vertices[i*vertexSize + offset + 0] = v.x;
					m_vertices[i*vertexSize + offset + 1] = v.y;
					m_vertices[i*vertexSize + offset + 2] = v.z;
				}
			}
			if (m_shaderInputLayout & SIL_NORMALMAP)
			{
				UINT offset1 = TangentOffset(m_shaderInputLayout);
				UINT offset2 = BinormalOffset(m_shaderInputLayout);
				for (UINT i = 0; i < getVertexCount(); i++)
				{
					v.x = m_vertices[i*vertexSize + offset1 + 0];
					v.y = m_vertices[i*vertexSize + offset1 + 1];
					v.z = m_vertices[i*vertexSize + offset1 + 2];
					v.w = 1;
					v = transform * v;
					m_vertices[i*vertexSize + offset1 + 0] = v.x;
					m_vertices[i*vertexSize + offset1 + 1] = v.y;
					m_vertices[i*vertexSize + offset1 + 2] = v.z;

					v.x = m_vertices[i*vertexSize + offset2 + 0];
					v.y = m_vertices[i*vertexSize + offset2 + 1];
					v.z = m_vertices[i*vertexSize + offset2 + 2];
					v.w = 1;
					v = transform * v;
					m_vertices[i*vertexSize + offset2 + 0] = v.x;
					m_vertices[i*vertexSize + offset2 + 1] = v.y;
					m_vertices[i*vertexSize + offset2 + 2] = v.z;
				}
			}
		}
	}
}