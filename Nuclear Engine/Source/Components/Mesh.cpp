#include "Components\Mesh.h"
#include "Core\Context.h"
#include <API\InputLayout.h>
#include <API\OpenGL\GLContext.h>

#include <iostream>

namespace NuclearEngine {

	namespace Components {
		
		Mesh::Mesh(MeshData _data, bool Initialize_) : data(_data)
		{
			if (Initialize_ == true)
			{
				this->Initialize();
			}
		}

		Mesh::Mesh(const Mesh& obj) : data(obj.data)
		{
		}

		Mesh::~Mesh()
		{
			API::VertexBuffer::Delete(&VBO);
			API::IndexBuffer::Delete(&IBO);
			/*for (size_t i = 0; i < data.textures.size(); i++)
			{
				API::Texture::Delete(&data.textures.at(i).Texture);
			}
			data.textures.clear();*/
		}

		void Mesh::Initialize()
		{
			VertexBufferDesc desc;
			desc.data = data.vertices.data();
			desc.size = (unsigned int)data.vertices.size() * sizeof(Vertex);
			desc.usage = BufferUsage::Static;

			PreCalculate_TextureBindings();

			API::VertexBuffer::Create(&VBO, &desc);
			API::IndexBuffer::Create(&IBO, data.indices.data(), data.indices.size());

			IndicesCount = data.indices.size();
			data.vertices.clear();
			data.indices.clear();
			
			Init = true;
		}

		void Mesh::Draw(API::Shader* _shader)
		{
			if (this->DrewBefore == false)
			{
				if (Init != true)
				{
					Initialize();
				}

				API::InputLayout layout;
				layout.Push("POSITION", 0, DataType::Float3);
				layout.Push("NORMAL", 0, DataType::Float3);
				layout.Push("TEXCOORD", 0, DataType::Float2);

				VBO.SetInputLayout(&layout, _shader);

				DrewBefore = true;
			}

			for (unsigned int i = 0; i < data.textures.size(); i++)
			{
				data.textures[i].Texture.PSBind(TextureBindings[i].c_str(), _shader, i);
			}
			
			// draw mesh
			VBO.Bind();
			IBO.Bind();
			Core::Context::DrawIndexed(IndicesCount);
		}

		void Mesh::PreCalculate_TextureBindings()
		{	
			for (unsigned int i = 0; i < data.textures.size(); i++)
			{

				std::string number;
				std::string name;

				if (data.textures[i].type == MeshTextureType::Diffuse)
				{
					name = "NE_Tex_Diffuse";
					number = std::to_string(diffuseNr++);
				}
				else if (data.textures[i].type == MeshTextureType::Specular)
				{
					name = "NE_Tex_Specular";
					number = std::to_string(specularNr++); // transfer unsigned int to stream
				}
				
				TextureBindings.push_back(name + number);
			}
		}
		
	}
}