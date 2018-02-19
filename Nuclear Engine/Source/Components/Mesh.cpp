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
			
		}

		void Mesh::Initialize()
		{
			API::VertexBufferDesc desc;
			desc.data = data.vertices.data();
			desc.size = (unsigned int)data.vertices.size() * sizeof(Vertex);
			desc.usage = API::BufferUsage::Static;

			API::VertexBuffer::Create(&VBO, desc);
			API::IndexBuffer::Create(&IBO, data.indices.data(), data.indices.size());

			IndicesCount = data.indices.size();
			data.vertices.clear();
			data.indices.clear();
			
			Init = true;
		}

		void Mesh::Release()
		{
			API::VertexBuffer::Delete(&VBO);
			API::IndexBuffer::Delete(&IBO);
			for (size_t i = 0; i < data.textures.size(); i++)
			{
				API::Texture::Delete(&data.textures.at(i).Texture);
			}
			data.textures.clear();
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
				layout.AppendAttribute("POSITION", 0, API::DataType::Float3);
				layout.AppendAttribute("NORMAL", 0, API::DataType::Float3);
				layout.AppendAttribute("TEXCOORD", 0, API::DataType::Float2);

				VBO.SetInputLayout(&layout, _shader);

				DrewBefore = true;
			}
			bool diffusebound = false;
			bool specularbound = false;
			for (unsigned int i = 0; i < data.textures.size(); i++)
			{

				if (data.textures[i].type == MeshTextureType::Diffuse)
				{
					if (diffusebound != true)
					{
						data.textures[i].Texture.PSBind(0);
						diffusebound = true;
					}
				}
				else if (data.textures[i].type == MeshTextureType::Specular)
				{
					if (specularbound != true)
					{
						data.textures[i].Texture.PSBind(1);
						specularbound = true;
					}
				}
			}
			
			// draw mesh
			VBO.Bind();
			IBO.Bind();
			Core::Context::DrawIndexed(IndicesCount);
		}
	}
}