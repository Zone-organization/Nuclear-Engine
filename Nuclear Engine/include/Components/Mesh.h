#pragma once
#include <Math\Math.h>
#include <API\Shader.h>
#include <API\Texture.h>
#include <API\VertexBuffer.h>
#include <API\IndexBuffer.h>
#include <vector>

namespace NuclearEngine {

	namespace Components {

		struct Vertex 
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

		enum class MaterialTextureType 
		{
			Diffuse,
			Specular
		};

		struct MaterialTexture {
			API::Texture *tex;
			MaterialTextureType type;
		};

		class NEAPI Mesh {
		public:

			/*  Mesh Data  */
			Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MaterialTexture> textures);
			Mesh(const Mesh& obj);

			void Initialize();
			// render the mesh
			void Draw(API::Shader* shader);

		private:
			//Calculate Texture binding slots and store them for faster drawing
			void PreCalculate_TextureBindings();
			std::vector<const char*> TextureBindings;

			//Buffers
			API::IndexBuffer IBO;
			API::VertexBuffer VBO;

			//Data containers
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			std::vector<MaterialTexture> textures;
			
			// bind appropriate textures
			unsigned int diffuseNr = 0;
			unsigned int specularNr = 0;

			bool DrewBefore = false;
		};
	}
}