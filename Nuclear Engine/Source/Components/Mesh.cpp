#include "Components\Mesh.h"
#include "Core\Context.h"
#include <API\InputLayout.h>
#include <iostream>

namespace NuclearEngine {

	namespace Components {
		
		Mesh::Mesh(MeshData _data)
			: data(_data)
		{
			Initialize();
		}

		Mesh::Mesh(const Mesh& obj)
			: data(obj.data)
		{
		}

		void Mesh::Initialize()
		{
			VertexBufferDesc desc;
			desc.data = data.vertices.data();
			desc.size = (unsigned int)data.vertices.size() * sizeof(Vertex);
			desc.usage = BufferUsage::Static;

			PreCalculate_TextureBindings();

			API::VertexBuffer::Create(&VBO, &desc);
			API::IndexBuffer::Create(&IBO, data.indices.data(), data.indices.size() * sizeof(unsigned int));
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

				Initialize();
				DrewBefore = true;
			}

			for (unsigned int i = 0; i < data.textures.size(); i++)
			{
				data.textures[i].tex->PSBind(TextureBindings[i], _shader, i);
			}
			
			// draw mesh

			IBO.Bind();
			VBO.Bind();
			Core::Context::DrawIndexed(data.indices.size());
		}

		void Mesh::PreCalculate_TextureBindings()
		{	
			for (unsigned int i = 0; i < data.textures.size(); i++)
			{

				std::string number;
				std::string name;

				if (data.textures[i].type == MaterialTextureType::Diffuse)
				{
					name = "NE_Tex_Diffuse";
					number = std::to_string(diffuseNr++);
				}
				else if (data.textures[i].type == MaterialTextureType::Specular)
				{
					name = "NE_Tex_Specular";
					number = std::to_string(specularNr++); // transfer unsigned int to stream
				}
				
				TextureBindings.push_back((name + number).c_str());
			}
		}
		
	}
}