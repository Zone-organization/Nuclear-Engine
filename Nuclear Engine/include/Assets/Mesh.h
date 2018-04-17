#pragma once
#include <Math\Math.h>
#include <Graphics/API/Texture.h>
#include <Graphics/API/VertexBuffer.h>
#include <Graphics/API/IndexBuffer.h>
#include <vector>
#include <string>

namespace NuclearEngine {
	namespace Assets {

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
		
		struct MeshVertexDesc
		{
			bool TexCoord = true;
			bool Normals = true;
			bool Tangents = false;
		};

		class NEAPI Mesh
		{
		public:			
			Mesh();
			~Mesh();
			/*
			Note:
				We only require shader to validate the input layout but you are free to render the model with any shader you want.
			*/
			void Initialize(Graphics::API::VertexShader* _shader);
			void Delete();

			static void CreateCube(Mesh* model, std::vector<MeshTexture> Textures, const MeshVertexDesc& desc = MeshVertexDesc(), float width = 1.0f, float  height = 1.0f, float  depth = 1.0f);
			static void CreateSphere(Mesh* model, std::vector<MeshTexture> Textures, const MeshVertexDesc& desc = MeshVertexDesc(), float radius = 0.5f, unsigned int sliceCount = 20, unsigned int stackCount = 20);
			static void CreatePlane(Mesh* model, std::vector<MeshTexture> Textures, const MeshVertexDesc& desc = MeshVertexDesc(), float width = 1.0f, float depth = 1.0f);
			static void CreateGrid(Mesh* model, std::vector<MeshTexture> Textures, const MeshVertexDesc& desc = MeshVertexDesc(), float width = 10.0f, float depth = 10.0f, unsigned int m = 10, unsigned int n = 10);
			static void CreateScreenQuad(Mesh* model);

			struct SubMesh {
				struct SubMeshData
				{
					std::vector<Math::Vector3> Positions;
					std::vector<Math::Vector2> UV;
					std::vector<Math::Vector3> Normals;
					std::vector<Math::Vector3> Tangents;

					std::vector<unsigned int> indices;
					std::vector<MeshTexture> textures;
				};

				/*  Mesh Data  */
				SubMesh(SubMeshData data);
				SubMesh(const SubMesh& obj);
				~SubMesh();
				void Initialize(Graphics::API::VertexShader* _shader);
				void Delete();

				SubMeshData data;
				unsigned int IndicesCount = 0;
				//Buffers
				Graphics::API::IndexBuffer IBO;
				Graphics::API::VertexBuffer VBO;
			};

			std::vector<SubMesh> SubMeshes;
		private:
			bool init;
		};

	}
}