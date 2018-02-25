#pragma once
#include <Math\Math.h>
#include <API\Texture.h>
#include <API\VertexBuffer.h>
#include <API\IndexBuffer.h>
#include <vector>
#include <string>

namespace NuclearEngine {
	namespace Assets {

		enum class NEAPI MeshTextureType
		{
			Diffuse,
			Specular
		};

		struct NEAPI MeshTexture {
			API::Texture Texture;
			MeshTextureType type;
		};
		struct NEAPI MeshData
		{
			std::vector<Math::Vector3> Positions;
			std::vector<Math::Vector2> UV;
			std::vector<Math::Vector3> Normals;

			std::vector<unsigned int> indices;
			std::vector<MeshTexture> textures;
		};
		class NEAPI Mesh {
		public:

			/*  Mesh Data  */
			Mesh(MeshData data);
			Mesh(const Mesh& obj);
			~Mesh();
			void Initialize(API::Shader* _shader);
			void Delete();

			MeshData data;
			unsigned int IndicesCount = 0;
			//Buffers
			API::IndexBuffer IBO;
			API::VertexBuffer VBO;
		};

		class NEAPI ModelAsset
		{
		public:			
			ModelAsset();
			~ModelAsset();
			/*
			Note:
				We only require shader to validate the input layout but you are free to render the model with any shader you want.
			*/
			void Initialize(API::Shader* _shader);
			void Delete();

			static void CreateCube(ModelAsset* model, std::vector<MeshTexture> Textures, float width = 1.0f, float  height = 1.0f, float  depth = 1.0f);
			static void CreateSphere(ModelAsset* model, std::vector<MeshTexture> Textures, float radius = 0.5f, unsigned int sliceCount = 20, unsigned int stackCount = 20);

			std::vector<Mesh> Meshes;
		private:
			bool init;
		};

	}
}