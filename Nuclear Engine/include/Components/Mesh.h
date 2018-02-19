#pragma once
#include <Math\Math.h>
#include <API\Shader.h>
#include <API\Texture.h>
#include <API\VertexBuffer.h>
#include <API\IndexBuffer.h>
#include <vector>

namespace NuclearEngine {

	namespace Components {

		struct NEAPI Vertex
		{
			Vertex()
			{
			}
			Vertex(Math::Vector3 pos, Math::Vector3 norm, Math::Vector2 uv)
			{
				Position = pos;
				Normal = norm;
				TexCoords = uv;
			}
			Vertex(
				float px, float py, float pz,
				float nx, float ny, float nz,
				float u, float v)
				: Position(px, py, pz), Normal(nx, ny, nz), TexCoords(u, v)
			{

			}
			// position
			Math::Vector3 Position;
			// normal
			Math::Vector3 Normal;
			// texCoords
			Math::Vector2 TexCoords;
		};

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
			MeshData(std::vector<Vertex> _vertices,
			std::vector<unsigned int> _indices,
			std::vector<MeshTexture> _textures)
				:vertices(_vertices), indices(_indices), textures(_textures)
			{}
			MeshData(){}

			//Data containers
			std::vector<Vertex> vertices;
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
			void Release();
			// render the mesh
			void Draw();

		private:
			// bind appropriate textures
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;

			MeshData data;
			unsigned int IndicesCount = 0;
			//Buffers
			API::IndexBuffer IBO;
			API::VertexBuffer VBO;
			bool Init = false;
			bool DrewBefore = false;
		};
	}
}