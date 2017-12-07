#pragma once
#include <Math\Math.h>
#include <API\Shader.h>
#include <API\Texture.h>
#include <API\VertexBuffer.h>
#include <API\IndexBuffer.h>
#include <vector>

namespace NuclearEngine {

	namespace Components {
		enum class NEAPI InputSignatures {
			Position,
			Position_Texcoord,
			Position_Normal_Texcoord
		};
		
		struct Vertex_PNT {
			Vertex_PNT(Math::Vector3 pos, Math::Vector3 norm, Math::Vector2 uv)
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
		struct Vertex_PT {
			Vertex_PT(Math::Vector3 pos, Math::Vector2 uv)
			{
				Position = pos;
				TexCoords = uv;
			}

			// position
			Math::Vector3 Position;
			// texCoords
			Math::Vector2 TexCoords;
		};
		struct Vertex_P {
			Vertex_P(Math::Vector3 pos)
			{
				Position = pos;
			}
			
			// position
			Math::Vector3 Position;
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

		class NEAPI Mesh_NoIndices {
		public:	
			Mesh_NoIndices();
			Mesh_NoIndices(std::vector<float> vertices, std::vector<MaterialTexture> textures, InputSignatures Signature);

			// render the mesh
			virtual void Draw(API::Shader* shader);

		protected:
			void PreCalculate_TextureBindings();

			API::VertexBuffer *VBO;
			std::vector<float> vertices;
			std::vector<MaterialTexture> textures;		
			std::vector<const char*> TextureBindings;

			// bind appropriate textures
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;

			InputSignatures signature;
			bool RenderInit;
		};

		class NEAPI Mesh : Mesh_NoIndices {
		public:

			/*  Mesh Data  */
			Mesh(std::vector<float> vertices, std::vector<unsigned int> indices, std::vector<MaterialTexture> textures, InputSignatures Signature);

			// render the mesh
			virtual void Draw(API::Shader* shader) override;

		private:
			API::IndexBuffer *IBO;
			std::vector<unsigned int> indices;
		};
	}
}