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
			// position
			Math::Vector3 Position;
			// normal
			Math::Vector3 Normal;
			// texCoords
			Math::Vector2 TexCoords;
			// tangent
			Math::Vector3 Tangent;
			// bitangent
			Math::Vector3 Bitangent;
		};

		struct MeshTexture {
			API::Texture2D *tex;
			const char* type;
			const char* path;
		};

		class NEAPI Mesh {
		public:
			/*  Mesh Data  */
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			std::vector<MeshTexture> textures;

			Mesh(API::Shader* shader, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures);

			// render the mesh
			void Draw();

		private:
			API::VertexBuffer *VBO;
			API::IndexBuffer *IBO;
			API::Shader* m_shader;
		};
	}
}