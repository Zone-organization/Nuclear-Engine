#include "Components\Mesh.h"
#include "Core\Context.h"
#include <API\InputLayout.h>

namespace NuclearEngine {

	namespace Components {
		/*  Functions  */
		// constructor

		Mesh::Mesh(API::Shader* _shader,std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<MeshTexture> _textures)
		{
			this->vertices = _vertices;
			this->indices = _indices;
			this->textures = _textures;
			this->m_shader = _shader;


			API::VertexBufferDesc desc;
			desc.data = &vertices[0];
			desc.size = (unsigned int)vertices.size() * sizeof(Vertex);
			desc.usage = API::BufferUsage::Static;

			VBO = new API::VertexBuffer(desc);

			IBO = new API::IndexBuffer(&this->indices[0], indices.size() * sizeof indices[0]);

			API::InputLayout layout;
			layout.Push(0, "Positions", API::DataType::Float3, sizeof(Vertex), 0);
			layout.Push(1, "Normals", API::DataType::Float3, sizeof(Vertex), offsetof(Vertex, Normal));
			layout.Push(2, "TexCoords", API::DataType::Float3, sizeof(Vertex), offsetof(Vertex, TexCoords));
			layout.Push(3, "Tangent", API::DataType::Float3, sizeof(Vertex), offsetof(Vertex, Tangent));
			layout.Push(4, "Bitangent", API::DataType::Float3, sizeof(Vertex), offsetof(Vertex, Bitangent));

			VBO->SetInputLayout(&layout, m_shader);
		}

		// render the mesh

		void Mesh::Draw()
		{
			// bind appropriate textures
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int heightNr = 1;
			for (unsigned int i = 0; i < textures.size(); i++)
			{

				std::string number;
				std::string name = textures[i].type;
				if (name == "texture_diffuse")
					number = std::to_string(diffuseNr++);
				else if (name == "texture_specular")
					number = std::to_string(specularNr++); // transfer unsigned int to stream
				else if (name == "texture_normal")
					number = std::to_string(normalNr++); // transfer unsigned int to stream
				else if (name == "texture_height")
					number = std::to_string(heightNr++); // transfer unsigned int to stream

				std::cout << (name + number).c_str();
				textures[i].tex->Bind((name + number).c_str(), m_shader, i);
			}

			// draw mesh
			VBO->Bind();
			IBO->Bind();
			Core::Context::DrawIndexed(indices.size());
			IBO->Unbind();
			VBO->Unbind();

			textures[0].tex->Unbind();
		}

	}
}