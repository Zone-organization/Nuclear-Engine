#pragma once
#include <Components\Mesh.h>
#include <string>
#include <vector>
#include <Shading\Material.h>

namespace NuclearEngine {
	namespace Components {

		class NEAPI Model
		{
		public:			
			Model();
			~Model();
			/*
			Note:
				We only require shader to validate the input layout but you are free to render the model with any shader you want.
			*/
			void Initialize(API::Shader* _shader);
			static void CreateCube(Model* model, std::vector<MeshTexture> Textures, float width = 1.0f, float  height = 1.0f, float  depth = 1.0f);
			static void CreateSphere(Model* model, std::vector<MeshTexture> Textures, float radius = 0.5f, unsigned int sliceCount = 20, unsigned int stackCount = 20);

			virtual void Draw();

			std::vector<Mesh> Meshes;
		};

	}
}