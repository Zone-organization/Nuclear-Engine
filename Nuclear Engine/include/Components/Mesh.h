#pragma once
#include <Math\Vector.h>
#include <API\Shader.h>
#include <API\Texture2D.h>
#include <API\VertexBuffer.h>
#include <API\IndexBuffer.h>
#include <vector>

namespace NuclearEngine {

	namespace Components {
		struct Vertex {
			Vertex(Math::Vector3 pos, Math::Vector3 norm, Math::Vector2 uv)
			{
				Position = pos;
				Normal = norm;
				TexCoords = uv;
			}
			// position
			Math::Vector3 Position;
			// normal
			Math::Vector3 Normal;
			// texCoords
			Math::Vector2 TexCoords;
		};

		struct MeshTexture {
			API::Texture2D *tex;
			const char* type;
			const char* path;
		};

		struct Material 
		{
			API::Texture2D *Diffuse;
			API::Texture2D *Specular;
		};


		class NEAPI Mesh_NoIndices {
		public:	
			Mesh_NoIndices();
			Mesh_NoIndices(std::vector<Vertex> vertices, std::vector<MeshTexture> textures);

			// render the mesh
			virtual void Draw(API::Shader* shader);

		protected:
			API::VertexBuffer *VBO;
			/*  Mesh Data  */
			std::vector<Vertex> vertices;
			std::vector<MeshTexture> textures;

			bool RenderInit;
		};

		class NEAPI Mesh : Mesh_NoIndices {
		public:

			/*  Mesh Data  */
			Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures);

			// render the mesh
			virtual void Draw(API::Shader* shader) override;

		private:
			API::IndexBuffer *IBO;
			std::vector<unsigned int> indices;
		};
	}
}