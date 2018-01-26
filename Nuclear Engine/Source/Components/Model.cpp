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
			for (unsigned int i = 0; i < Meshes.size(); i++)
			{
				Meshes.at(i).Release();
			}
			Meshes.clear();
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