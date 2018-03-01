#include "XAsset\ModelAsset.h"
#include "Core\Context.h"
#include <API\InputLayout.h>

namespace NuclearEngine {

	namespace XAsset {

		Mesh::Mesh(MeshData _data) : data(_data)
		{
		}

		Mesh::Mesh(const Mesh& obj) : data(obj.data)
		{
		}

		Mesh::~Mesh()
		{

		}

		void Mesh::Initialize(API::VertexShader* _shader)
		{
			std::vector<float> verticesdata;
			for (unsigned int i = 0; i < data.Positions.size(); ++i)
			{
				verticesdata.push_back(data.Positions[i].x);
				verticesdata.push_back(data.Positions[i].y);
				verticesdata.push_back(data.Positions[i].z);
				if (data.UV.size() > 0)
				{
					verticesdata.push_back(data.UV[i].x);
					verticesdata.push_back(data.UV[i].y);
				}
				if (data.Normals.size() > 0)
				{
					verticesdata.push_back(data.Normals[i].x);
					verticesdata.push_back(data.Normals[i].y);
					verticesdata.push_back(data.Normals[i].z);
				}
				if (data.Tangents.size() > 0)
				{
					verticesdata.push_back(data.Tangents[i].x);
					verticesdata.push_back(data.Tangents[i].y);
					verticesdata.push_back(data.Tangents[i].z);
				}
			}
			API::VertexBufferDesc desc;
			desc.data = verticesdata.data();
			desc.size = (unsigned int)verticesdata.size() * sizeof(float);
			desc.usage = API::BufferUsage::Static;

			API::VertexBuffer::Create(&VBO, desc);
			API::IndexBuffer::Create(&IBO, data.indices.data(), data.indices.size());

			IndicesCount = data.indices.size();

			API::InputLayout layout;
			layout.AppendAttribute("POSITION", 0, API::DataType::Float3);
			if (data.UV.size() > 0)
			{
				layout.AppendAttribute("TEXCOORD", 0, API::DataType::Float2);
			}
			if (data.Normals.size() > 0)
			{
				layout.AppendAttribute("NORMAL", 0, API::DataType::Float3);
			}
			if (data.Tangents.size() > 0)
			{
				layout.AppendAttribute("TANGENT", 0, API::DataType::Float3);
			}
			VBO.SetInputLayout(&layout, _shader);

			data.Positions.clear();
			data.UV.clear();
			data.Normals.clear();
			data.Tangents.clear();
			data.indices.clear();
		}

		void Mesh::Delete()
		{
			API::VertexBuffer::Delete(&VBO);
			API::IndexBuffer::Delete(&IBO);
			for (size_t i = 0; i < data.textures.size(); i++)
			{
				API::Texture::Delete(&data.textures.at(i).Texture);
			}
			data.textures.clear();
		}

		ModelAsset::ModelAsset()
		{

		}

		ModelAsset::~ModelAsset()
		{
			for (unsigned int i = 0; i < Meshes.size(); i++)
			{
				Meshes.at(i).Delete();
			}
			Meshes.clear();
		}

		void ModelAsset::Initialize(API::VertexShader* _shader)
		{
			for (unsigned int i = 0; i < Meshes.size(); i++)
			{
				Meshes.at(i).Initialize(_shader);
			}
			init = true;
		}
		struct Vertex
		{
			Vertex() {}
			Vertex(const Math::Vector3& p, const Math::Vector3& n, const Math::Vector3& t, const Math::Vector2& uv)
				: Position(p), Normal(n), Tangents(t), UV(uv) {}
			Vertex(
				float px, float py, float pz,
				float nx, float ny, float nz,
				float tx, float ty, float tz,
				float u, float v)
				: Position(px, py, pz), Normal(nx, ny, nz),
				Tangents(tx, ty, tz), UV(u, v) {}

			Math::Vector3 Position;
			Math::Vector2 UV;
			Math::Vector3 Normal;
			Math::Vector3 Tangents;
		};

		//Todo rework this since we do alot of unnecessery looping
		void ModelAsset::CreateCube(ModelAsset* model, std::vector<MeshTexture> Textures, const ModelAssetVertexDesc& desc, float width, float height, float depth)
		{
			Vertex v[24];

			float w2 = 0.5f*width;
			float h2 = 0.5f*height;
			float d2 = 0.5f*depth;

			// Fill in the front face vertex data.
			v[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			v[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			v[2] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
			v[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

			// Fill in the back face vertex data.
			v[4] = Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
			v[5] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			v[6] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			v[7] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

			// Fill in the top face vertex data.
			v[8] = Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			v[9] = Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			v[10] = Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
			v[11] = Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

			// Fill in the bottom face vertex data.
			v[12] = Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
			v[13] = Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			v[14] = Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			v[15] = Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

			// Fill in the left face vertex data.
			v[16] = Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
			v[17] = Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
			v[18] = Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
			v[19] = Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

			// Fill in the right face vertex data.
			v[20] = Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
			v[21] = Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
			v[22] = Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
			v[23] = Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

			std::vector<Vertex> vertices;
			vertices.assign(&v[0], &v[24]);

			MeshData meshData;

			for (Vertex vert : vertices)
			{
				meshData.Positions.push_back(vert.Position);
				if (desc.TexCoord == true)
					meshData.UV.push_back(vert.UV);
				if (desc.Normals == true)
					meshData.Normals.push_back(vert.Normal);
				if (desc.Tangents == true)
					meshData.Tangents.push_back(vert.Tangents);
			}
			// Create the indices.			
			UINT i[36];
			// Fill in the front face index data
			i[0] = 0; i[1] = 1; i[2] = 2;
			i[3] = 0; i[4] = 2; i[5] = 3;
			// Fill in the back face index data
			i[6] = 4; i[7] = 5; i[8] = 6;
			i[9] = 4; i[10] = 6; i[11] = 7;
			// Fill in the top face index data
			i[12] = 8; i[13] = 9; i[14] = 10;
			i[15] = 8; i[16] = 10; i[17] = 11;
			// Fill in the bottom face index data
			i[18] = 12; i[19] = 13; i[20] = 14;
			i[21] = 12; i[22] = 14; i[23] = 15;
			// Fill in the left face index data
			i[24] = 16; i[25] = 17; i[26] = 18;
			i[27] = 16; i[28] = 18; i[29] = 19;
			// Fill in the right face index data
			i[30] = 20; i[31] = 21; i[32] = 22;
			i[33] = 20; i[34] = 22; i[35] = 23;

			std::vector<unsigned int> indices;
			indices.assign(&i[0], &i[36]);

			meshData.indices = indices;
			meshData.textures = Textures;

			model->Meshes.push_back(meshData);
		}

		void ModelAsset::CreateSphere(ModelAsset * model, std::vector<MeshTexture> Textures, const ModelAssetVertexDesc& desc, float radius, unsigned int sliceCount, unsigned int stackCount)
		{
			MeshData meshData;

			Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			std::vector<Vertex> vertices;

			vertices.push_back(topVertex);

			float phiStep = static_cast<float>(MathPI) / stackCount;
			float thetaStep = 2.0f * static_cast<float>(MathPI) / sliceCount;

			// Compute vertices for each stack ring (do not count the poles as rings).
			for (UINT i = 1; i <= stackCount - 1; ++i)
			{
				float phi = i * phiStep;

				// Vertices of ring.
				for (UINT j = 0; j <= sliceCount; ++j)
				{
					float theta = j * thetaStep;

					Vertex v;

					// spherical to cartesian
					v.Position.x = radius * sinf(phi)*cosf(theta);
					v.Position.y = radius * cosf(phi);
					v.Position.z = radius * sinf(phi)*sinf(theta);

					Math::Vector3 p = v.Position;
					v.Normal = Math::Normalize(p);

					// Partial derivative of P with respect to theta
					v.Tangents.x = -radius * sinf(phi)*sinf(theta);
					v.Tangents.y = 0.0f;
					v.Tangents.z = +radius * sinf(phi)*cosf(theta);

					Math::Vector3 T = v.Tangents;
					v.Tangents = Math::Normalize(T);

					v.UV.x = theta / (2 * static_cast<float>(MathPI));
					v.UV.y = phi / static_cast<float>(MathPI);

					vertices.push_back(v);
				}
			}

			vertices.push_back(bottomVertex);

			for (UINT i = 1; i <= sliceCount; ++i)
			{
				meshData.indices.push_back(0);
				meshData.indices.push_back(i + 1);
				meshData.indices.push_back(i);
			}
			UINT baseIndex = 1;
			UINT ringVertexCount = sliceCount + 1;
			for (UINT i = 0; i < stackCount - 2; ++i)
			{
				for (UINT j = 0; j < sliceCount; ++j)
				{
					meshData.indices.push_back(baseIndex + i * ringVertexCount + j);
					meshData.indices.push_back(baseIndex + i * ringVertexCount + j + 1);
					meshData.indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

					meshData.indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
					meshData.indices.push_back(baseIndex + i * ringVertexCount + j + 1);
					meshData.indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
				}
			}

			UINT southPoleIndex = (UINT)vertices.size() - 1;

			// Offset the indices to the index of the first vertex in the last ring.
			baseIndex = southPoleIndex - ringVertexCount;

			for (UINT i = 0; i < sliceCount; ++i)
			{
				meshData.indices.push_back(southPoleIndex);
				meshData.indices.push_back(baseIndex + i);
				meshData.indices.push_back(baseIndex + i + 1);
			}
			meshData.textures = Textures;

			for (Vertex vert : vertices)
			{
				meshData.Positions.push_back(vert.Position);
				if (desc.TexCoord == true)
					meshData.UV.push_back(vert.UV);
				if (desc.Normals == true)
					meshData.Normals.push_back(vert.Normal);
				if (desc.Tangents == true)
					meshData.Tangents.push_back(vert.Tangents);
			}
			model->Meshes.push_back(meshData);
		}

		void ModelAsset::CreateGrid(ModelAsset * model, std::vector<MeshTexture> Textures, const ModelAssetVertexDesc & desc, float width, float depth, unsigned int m, unsigned int n)
		{
			MeshData meshData;
			std::vector<Vertex> Vertices;
			std::vector<uint> Indices;

			UINT vertexCount = m * n;
			UINT faceCount = (m - 1)*(n - 1) * 2;

			//
			// Create the vertices.
			//

			float halfWidth = 0.5f*width;
			float halfDepth = 0.5f*depth;

			float dx = width / (n - 1);
			float dz = depth / (m - 1);

			float du = 1.0f / (n - 1);
			float dv = 1.0f / (m - 1);

			Vertices.resize(vertexCount);
			for (UINT i = 0; i < m; ++i)
			{
				float z = halfDepth - i * dz;
				for (UINT j = 0; j < n; ++j)
				{
					float x = -halfWidth + j * dx;

					Vertices[i*n + j].Position = Math::Vector3(x, 0.0f, z);
					Vertices[i*n + j].Normal = Math::Vector3(0.0f, 1.0f, 0.0f);
					Vertices[i*n + j].Tangents = Math::Vector3(1.0f, 0.0f, 0.0f);

					// Stretch texture over grid.
					Vertices[i*n + j].UV.x = j * du;
					Vertices[i*n + j].UV.y = i * dv;
				}
			}

			//
			// Create the indices.
			//

			Indices.resize(faceCount * 3); // 3 indices per face

													// Iterate over each quad and compute indices.
			UINT k = 0;
			for (UINT i = 0; i < m - 1; ++i)
			{
				for (UINT j = 0; j < n - 1; ++j)
				{
					Indices[k] = i * n + j;
					Indices[k + 1] = i * n + j + 1;
					Indices[k + 2] = (i + 1)*n + j;

					Indices[k + 3] = (i + 1)*n + j;
					Indices[k + 4] = i * n + j + 1;
					Indices[k + 5] = (i + 1)*n + j + 1;

					k += 6; // next quad
				}
			}

			for (Vertex vert : Vertices)
			{
				meshData.Positions.push_back(vert.Position);
				if (desc.TexCoord == true)
					meshData.UV.push_back(vert.UV);
				if (desc.Normals == true)
					meshData.Normals.push_back(vert.Normal);
				if (desc.Tangents == true)
					meshData.Tangents.push_back(vert.Tangents);
			}
			meshData.indices = Indices;

			meshData.textures = Textures;

			model->Meshes.push_back(meshData);
		}

		void ModelAsset::CreateScreenQuad(ModelAsset * model)
		{
			std::vector<Vertex> Vertices;
			std::vector<uint> Indices;

			MeshData meshData;

			Vertices.resize(4);
			Indices.resize(6);

			float VTexCoord = +1.0f;

			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				VTexCoord = -1.0f;
			}
			// Position coordinates specified in NDC space.
			Vertices[0] = Vertex(
				-1.0f, -1.0f, 0.0f,
				0.0f, 0.0f, -1.0f,
				1.0f, 0.0f, 0.0f,
				0.0f, VTexCoord);

			Vertices[1] = Vertex(
				-1.0f, +1.0f, 0.0f,
				0.0f, 0.0f, -1.0f,
				1.0f, 0.0f, 0.0f,
				0.0f, 0.0f);

			Vertices[2] = Vertex(
				+1.0f, +1.0f, 0.0f,
				0.0f, 0.0f, -1.0f,
				1.0f, 0.0f, 0.0f,
				1.0f, 0.0f);

			Vertices[3] = Vertex(
				+1.0f, -1.0f, 0.0f,
				0.0f, 0.0f, -1.0f,
				1.0f, 0.0f, 0.0f,
				1.0f, VTexCoord);

			Indices[0] = 0;
			Indices[1] = 1;
			Indices[2] = 2;

			Indices[3] = 0;
			Indices[4] = 2;
			Indices[5] = 3;

			for (Vertex vert : Vertices)
			{
				meshData.Positions.push_back(vert.Position);
				meshData.UV.push_back(vert.UV);
			}
			meshData.indices = Indices;

			model->Meshes.push_back(meshData);
		}

	}
}