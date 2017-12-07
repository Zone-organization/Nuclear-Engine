#include "Components\Mesh.h"
#include "Core\Context.h"
#include <API\InputLayout.h>

namespace NuclearEngine {

	namespace Components {
		/*  Functions  */

		unsigned int GetVerticesSize(InputSignatures Signature)
		{
			switch (Signature)
			{
			case InputSignatures::Position: return sizeof(Vertex_P);
			case InputSignatures::Position_Texcoord: return sizeof(Vertex_PT);
			case InputSignatures::Position_Normal_Texcoord: return sizeof(Vertex_PNT);
			default: return sizeof(Vertex_PNT);
			}
		}

		// constructor
		Mesh::Mesh(std::vector<float> _vertices, std::vector<unsigned int> _indices, std::vector<MaterialTexture> _textures, InputSignatures Signature)
		{
			Mesh_NoIndices::Mesh_NoIndices(_vertices, _textures, Signature);

			this->indices = _indices;
			IBO = new API::IndexBuffer(&this->indices[0], indices.size() * sizeof indices[0]);			
		}

		// render the mesh
		void Mesh_NoIndices::PreCalculate_TextureBindings()
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

		void Mesh::Draw(API::Shader* _shader)
		{
			if (this->RenderInit != false)
			{
				API::InputLayout layout;
				layout.Push("POSITION", 0, DataType::Float3);
				layout.Push("NORMAL", 0, DataType::Float);
				layout.Push("TEXCOORD", 0, DataType::Float2);
	
				VBO->SetInputLayout(&layout, _shader);
			}
			// bind appropriate textures
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int heightNr = 1;
			for (unsigned int i = 0; i < textures.size(); i++)
			{

				std::string number;
				//std::string name = textures[i].type;
				//if (name == "texture_diffuse")
				//	number = std::to_string(diffuseNr++);
				//else if (name == "texture_specular")
				//	number = std::to_string(specularNr++); // transfer unsigned int to stream

				//std::cout << (name + number).c_str();
				//textures[i].tex->PSBind((name + number).c_str(), _shader, i);
			}

			// draw mesh
			VBO->Bind();
			IBO->Bind();
			Core::Context::DrawIndexed(indices.size());
			IBO->Unbind();
			VBO->Unbind();
		}

		Mesh_NoIndices::Mesh_NoIndices()
		{
		}

		Mesh_NoIndices::Mesh_NoIndices(std::vector<float> _vertices, std::vector<MaterialTexture> _textures, InputSignatures Signature)
		{
			this->vertices = _vertices;
			this->textures = _textures;

			this->signature = Signature;

			API::VertexBufferDesc desc;
			desc.data = vertices.data();
			desc.size = (unsigned int)vertices.size() * sizeof(float);
			desc.usage = BufferGPUUsage::Dynamic;
			desc.accessflag = BufferCPUAccess::Default;
			
			VBO = new API::VertexBuffer(desc);

			PreCalculate_TextureBindings();

			RenderInit = false;
		}

		void Mesh_NoIndices::Draw(API::Shader * shader)
		{

			if (this->RenderInit == false)
			{
				API::InputLayout layout;
				layout.Push("POSITION", 0, DataType::Float3);

				if (signature == InputSignatures::Position_Texcoord)
				{
					layout.Push("TEXCOORD", 0, DataType::Float2);
				}
				else if (signature == InputSignatures::Position_Normal_Texcoord)
				{
					layout.Push("NORMAL", 0, DataType::Float3);
					layout.Push("TEXCOORD", 0, DataType::Float2);
				}
				VBO->SetInputLayout(&layout, shader);

				RenderInit = true;
			}

			for (unsigned int i = 0; i < textures.size(); i++)
			{
				textures[i].tex->PSBind(TextureBindings[i], shader, i);
			}

			// draw mesh
			VBO->Bind();
			Core::Context::Draw(vertices.size());
			VBO->Unbind();

		}
		
	}
}