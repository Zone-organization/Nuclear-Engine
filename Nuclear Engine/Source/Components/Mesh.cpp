#include "Components\Mesh.h"
#include "Core\Context.h"
#include <API\InputLayout.h>

namespace NuclearEngine {

	namespace Components {
		
		// constructor
		Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<MaterialTexture> _textures)
		{
			this->vertices = _vertices;
			this->textures = _textures;
			this->indices = _indices;
			DrewBefore = false;
		}

		Mesh::Mesh(const Mesh& obj)
		{
			this->vertices = obj.vertices;
			this->textures = obj.textures;
			this->indices = obj.indices;
			this->Initialize();				
		}

		void Mesh::Initialize()
		{
			VertexBufferDesc desc;
			desc.data = vertices.data();
			desc.size = (unsigned int)vertices.size() * sizeof(Vertex);
			desc.usage = BufferUsage::Static;

			PreCalculate_TextureBindings();

			API::VertexBuffer::Create(&VBO, &desc);
			API::IndexBuffer::Create(&IBO, indices.data(), indices.size() * sizeof(unsigned int));		
		}
		void Mesh::Draw(API::Shader* _shader)
		{
			if (this->DrewBefore == false)
			{
				API::InputLayout layout;
				layout.Push("POSITION", 0, DataType::Float3);
				layout.Push("NORMAL", 0, DataType::Float3);
				layout.Push("TEXCOORD", 0, DataType::Float2);
				
				VBO.SetInputLayout(&layout, _shader);

				DrewBefore = true;
			}

			for (unsigned int i = 0; i < textures.size(); i++)
			{
				textures[i].tex->PSBind(TextureBindings[i], _shader, i);
			}
			
			// draw mesh
			IBO.Bind();
			VBO.Bind();
			Core::Context::DrawIndexed(indices.size());
		}

		void Mesh::PreCalculate_TextureBindings()
		{	
			for (unsigned int i = 0; i < textures.size(); i++)
			{

				std::string number;
				std::string name;

				if (textures[i].type == MaterialTextureType::Diffuse)
				{
					name = "NE_Tex_Diffuse";
					number = std::to_string(diffuseNr++);
				}
				else if (textures[i].type == MaterialTextureType::Specular)
				{
					name = "NE_Tex_Specular";
					number = std::to_string(specularNr++); // transfer unsigned int to stream
				}
				
				TextureBindings.push_back((name + number).c_str());
			}
		}
		
	}
}