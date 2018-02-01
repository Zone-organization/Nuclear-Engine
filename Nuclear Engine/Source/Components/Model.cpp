#include "Components\Model.h"
#include <AssetManager/AssetManager.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

using namespace std;
namespace NuclearEngine {

	namespace Components {

		Model::Model() 
		{
		
		}

		Model::~Model()
		{
			/*for (unsigned int i = 0; i < Meshes.size(); i++)
			{
				Meshes.at(i).Release();
			}
			Meshes.clear();*/
		}

		void Model::CreateCube(Model* model, std::vector<MeshTexture> Textures, float width , float height , float depth)
		{
			Vertex v[24];

			float w2 = 0.5f*width;
			float h2 = 0.5f*height;
			float d2 = 0.5f*depth;

			// Fill in the front face vertex data.
			v[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
			v[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
			v[2] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
			v[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
			// Fill in the back face vertex data.
			v[4] = Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
			v[5] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
			v[6] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
			v[7] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
			// Fill in the top face vertex data.
			v[8] = Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
			v[9] = Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			v[10] = Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
			v[11] = Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
			// Fill in the bottom face vertex data.
			v[12] = Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
			v[13] = Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
			v[14] = Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
			v[15] = Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
			// Fill in the left face vertex data.
			v[16] = Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			v[17] = Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			v[18] = Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
			v[19] = Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
			// Fill in the right face vertex data.
			v[20] = Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			v[21] = Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			v[22] = Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
			v[23] = Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

			std::vector<Vertex> vertices;
			vertices.assign(&v[0], &v[24]);

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

			model->Meshes.push_back(MeshData(vertices, indices, Textures));
		}

		void Model::CreateSphere(Model * model, std::vector<MeshTexture> Textures, float radius, unsigned int sliceCount, unsigned int stackCount)
		{
			MeshData meshData;

			Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, 0.0f);
			Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);

			meshData.vertices.push_back(topVertex);

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

					v.TexCoords.x = theta / (2 * static_cast<float>(MathPI));
					v.TexCoords.y = phi / static_cast<float>(MathPI);

					meshData.vertices.push_back(v);
				}
			}

			meshData.vertices.push_back(bottomVertex);

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

			UINT southPoleIndex = (UINT)meshData.vertices.size() - 1;

			// Offset the indices to the index of the first vertex in the last ring.
			baseIndex = southPoleIndex - ringVertexCount;

			for (UINT i = 0; i < sliceCount; ++i)
			{
				meshData.indices.push_back(southPoleIndex);
				meshData.indices.push_back(baseIndex + i);
				meshData.indices.push_back(baseIndex + i + 1);
			}
			meshData.textures = Textures;

			model->Meshes.push_back(meshData);
		}

		void Model::Draw(API::Shader* shader)
		{
			shader->Bind();
			for (unsigned int i = 0; i < Meshes.size(); i++)
			{
				Meshes[i].Draw(shader);
			}
		}

	}
}