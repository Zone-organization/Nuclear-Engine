#pragma once
#include <Math\Math.h>
#include <Graphics/API/Texture.h>
#include <Graphics/API/VertexBuffer.h>
#include <Graphics/API/IndexBuffer.h>
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
			Graphics::API::Texture Texture;
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
			void Initialize(Graphics::API::VertexShader* _shader);
			void Delete();

			MeshData data;
			unsigned int IndicesCount = 0;
			//Buffers
			Graphics::API::IndexBuffer IBO;
			Graphics::API::VertexBuffer VBO;
		};
		struct ModelAssetVertexDesc
		{
			bool TexCoord = true;
			bool Normals = true;
			bool Tangents = false;
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
			void Initialize(Graphics::API::VertexShader* _shader);
			void Delete();

			static void CreateCube(ModelAsset* model, std::vector<MeshTexture> Textures, const ModelAssetVertexDesc& desc = ModelAssetVertexDesc(), float width = 1.0f, float  height = 1.0f, float  depth = 1.0f);
			static void CreateSphere(ModelAsset* model, std::vector<MeshTexture> Textures, const ModelAssetVertexDesc& desc = ModelAssetVertexDesc(), float radius = 0.5f, unsigned int sliceCount = 20, unsigned int stackCount = 20);
			static void CreatePlane(ModelAsset* model, std::vector<MeshTexture> Textures, const ModelAssetVertexDesc& desc = ModelAssetVertexDesc(), float width = 160.0f, float depth = 160.0f, unsigned int m = 50, unsigned int n = 50);
			static void CreateGrid(ModelAsset* model, std::vector<MeshTexture> Textures, const ModelAssetVertexDesc& desc = ModelAssetVertexDesc(), float width = 10.0f, float depth = 10.0f, unsigned int m = 10, unsigned int n = 10);
			static void CreateScreenQuad(ModelAsset* model);

			std::vector<Mesh> Meshes;
		private:
			bool init;
		};

	}
}