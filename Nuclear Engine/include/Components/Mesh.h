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

		enum class NEAPI MaterialTextureType
		{
			Diffuse,
			Specular
		};

		struct NEAPI MaterialTexture {
			API::Texture *tex;
			MaterialTextureType type;
		};
		struct NEAPI MeshData
		{
			MeshData(std::vector<Vertex> _vertices,
			std::vector<unsigned int> _indices,
			std::vector<MaterialTexture> _textures)
				:vertices(_vertices), indices(_indices), textures(_textures)
			{}

			//Data containers
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			std::vector<MaterialTexture> textures;
		};
		class NEAPI Mesh {
		public:

			/*  Mesh Data  */
			Mesh(MeshData data);
			Mesh(const Mesh& obj);

			void Initialize();

			// render the mesh
			void Draw(API::Shader* shader);

		private:
			//Calculate Texture binding slots and store them for faster drawing
			void PreCalculate_TextureBindings();
			std::vector<const char*> TextureBindings;
			// bind appropriate textures
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;

			MeshData data;

			//Buffers
			API::IndexBuffer IBO;
			API::VertexBuffer VBO;
			bool Init = false;
			bool DrewBefore = false;
		};
	}
}