#pragma once
#include <Math\Math.h>
#include <API\Texture.h>
#include <API\VertexBuffer.h>
#include <API\IndexBuffer.h>
#include <vector>
#include <string>

namespace NuclearEngine {
	//XAsset the X here stands for the asset class as "ModelAsset" or "FontAsset" 
	//Basically, I didn't find a better name and avoid XAsset to prevent confusion since these aren't real assets
	namespace XAsset {

		enum class NEAPI MeshTextureType
		{
			Diffuse,
			Specular,
			Normal
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
			std::vector<Math::Vector3> Tangents;

			std::vector<unsigned int> indices;
			std::vector<MeshTexture> textures;
		};
		class NEAPI Mesh {
		public:

			/*  Mesh Data  */
			Mesh(MeshData data);
			Mesh(const Mesh& obj);
			~Mesh();
			void Initialize(API::VertexShader* _shader);
			void Delete();

			MeshData data;
			unsigned int IndicesCount = 0;
			//Buffers
			API::IndexBuffer IBO;
			API::VertexBuffer VBO;
		};
		struct ModelAssetVertexDesc
		{
			bool TexCoord = true;
			bool Normals = true;
			bool Tangents = false;
		};

		class NEAPI ScreenQuadAsset {
		public:
			void Initialize(API::VertexShader* _shader);
			void Render();

		private:
			API::VertexBuffer ScreenQuadVB;
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
			void Initialize(API::VertexShader* _shader);
			void Delete();

			static void CreateCube(ModelAsset* model, std::vector<MeshTexture> Textures, const ModelAssetVertexDesc& desc = ModelAssetVertexDesc(), float width = 1.0f, float  height = 1.0f, float  depth = 1.0f);
			static void CreateSphere(ModelAsset* model, std::vector<MeshTexture> Textures, const ModelAssetVertexDesc& desc = ModelAssetVertexDesc(), float radius = 0.5f, unsigned int sliceCount = 20, unsigned int stackCount = 20);

			std::vector<Mesh> Meshes;
		private:
			bool init;
		};

	}
}