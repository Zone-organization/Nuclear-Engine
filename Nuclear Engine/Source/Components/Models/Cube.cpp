#include <Components\Models\Cube.h>

namespace NuclearEngine {
	namespace Components {
		std::vector<Math::Vector3> Positions = {
			Math::Vector3(-0.5f, -0.5f, -0.5f),
			Math::Vector3(0.5f, -0.5f, -0.5f),
			Math::Vector3(0.5f,  0.5f, -0.5f),
			Math::Vector3(0.5f,  0.5f, -0.5f),
			Math::Vector3(-0.5f,  0.5f, -0.5f),
			Math::Vector3(-0.5f, -0.5f, -0.5f),

			Math::Vector3(-0.5f, -0.5f,  0.5f),
			Math::Vector3(0.5f,  0.5f,  0.5f),
			Math::Vector3(0.5f, -0.5f,  0.5f),
			Math::Vector3(0.5f,  0.5f,  0.5f),
			Math::Vector3(-0.5f, -0.5f,  0.5f),
			Math::Vector3(-0.5f,  0.5f,  0.5f),

			Math::Vector3(-0.5f,  0.5f,  0.5f),
			Math::Vector3(-0.5f, -0.5f, -0.5f),
			Math::Vector3(-0.5f,  0.5f, -0.5f),
			Math::Vector3(-0.5f, -0.5f, -0.5f),
			Math::Vector3(-0.5f,  0.5f,  0.5f),
			Math::Vector3(-0.5f, -0.5f,  0.5f),

			Math::Vector3(0.5f,  0.5f,  0.5f),
			Math::Vector3(0.5f,  0.5f, -0.5f),
			Math::Vector3(0.5f, -0.5f, -0.5f),
			Math::Vector3(0.5f, -0.5f, -0.5f),
			Math::Vector3(0.5f, -0.5f,  0.5f),
			Math::Vector3(0.5f,  0.5f,  0.5f),

			Math::Vector3(-0.5f, -0.5f, -0.5f),
			Math::Vector3(0.5f, -0.5f,  0.5f),
			Math::Vector3(0.5f, -0.5f, -0.5f),
			Math::Vector3(0.5f, -0.5f,  0.5f),
			Math::Vector3(-0.5f, -0.5f, -0.5f),
			Math::Vector3(-0.5f, -0.5f,  0.5f),

			Math::Vector3(-0.5f,  0.5f, -0.5f),
			Math::Vector3(0.5f,  0.5f, -0.5f),
			Math::Vector3(0.5f,  0.5f,  0.5f),
			Math::Vector3(0.5f,  0.5f,  0.5f),
			Math::Vector3(-0.5f,  0.5f,  0.5f),
			Math::Vector3(-0.5f,  0.5f, -0.5f),
		};
		std::vector<Math::Vector2> UV = {
			Math::Vector2(0.0f, 0.0f),
			Math::Vector2(1.0f, 0.0f),
			Math::Vector2(1.0f, 1.0f),
			Math::Vector2(1.0f, 1.0f),
			Math::Vector2(0.0f, 1.0f),
			Math::Vector2(0.0f, 0.0f),

			Math::Vector2(0.0f, 0.0f),
			Math::Vector2(1.0f, 1.0f),
			Math::Vector2(1.0f, 0.0f),
			Math::Vector2(1.0f, 1.0f),
			Math::Vector2(0.0f, 0.0f),
			Math::Vector2(0.0f, 1.0f),

			Math::Vector2(1.0f, 0.0f),
			Math::Vector2(0.0f, 1.0f),
			Math::Vector2(1.0f, 1.0f),
			Math::Vector2(0.0f, 1.0f),
			Math::Vector2(1.0f, 0.0f),
			Math::Vector2(0.0f, 0.0f),

			Math::Vector2(1.0f, 0.0f),
			Math::Vector2(1.0f, 1.0f),
			Math::Vector2(0.0f, 1.0f),
			Math::Vector2(0.0f, 1.0f),
			Math::Vector2(0.0f, 0.0f),
			Math::Vector2(1.0f, 0.0f),

			Math::Vector2(0.0f, 1.0f),
			Math::Vector2(1.0f, 0.0f),
			Math::Vector2(1.0f, 1.0f),
			Math::Vector2(1.0f, 0.0f),
			Math::Vector2(0.0f, 1.0f),
			Math::Vector2(0.0f, 0.0f),

			Math::Vector2(0.0f, 1.0f),
			Math::Vector2(1.0f, 1.0f),
			Math::Vector2(1.0f, 0.0f),
			Math::Vector2(1.0f, 0.0f),
			Math::Vector2(0.0f, 0.0f),
			Math::Vector2(0.0f, 1.0f),
		};
		std::vector<Math::Vector3> Normals = {
			Math::Vector3(0.0f,  0.0f, -1.0f),
			Math::Vector3(0.0f,  0.0f, -1.0f),
			Math::Vector3(0.0f,  0.0f, -1.0f),
			Math::Vector3(0.0f,  0.0f, -1.0f),
			Math::Vector3(0.0f,  0.0f, -1.0f),
			Math::Vector3(0.0f,  0.0f, -1.0f),

			Math::Vector3(0.0f,  0.0f,  1.0f),
			Math::Vector3(0.0f,  0.0f,  1.0f),
			Math::Vector3(0.0f,  0.0f,  1.0f),
			Math::Vector3(0.0f,  0.0f,  1.0f),
			Math::Vector3(0.0f,  0.0f,  1.0f),
			Math::Vector3(0.0f,  0.0f,  1.0f),

			Math::Vector3(-1.0f,  0.0f,  0.0f),
			Math::Vector3(-1.0f,  0.0f,  0.0f),
			Math::Vector3(-1.0f,  0.0f,  0.0f),
			Math::Vector3(-1.0f,  0.0f,  0.0f),
			Math::Vector3(-1.0f,  0.0f,  0.0f),
			Math::Vector3(-1.0f,  0.0f,  0.0f),

			Math::Vector3(1.0f,  0.0f,  0.0f),
			Math::Vector3(1.0f,  0.0f,  0.0f),
			Math::Vector3(1.0f,  0.0f,  0.0f),
			Math::Vector3(1.0f,  0.0f,  0.0f),
			Math::Vector3(1.0f,  0.0f,  0.0f),
			Math::Vector3(1.0f,  0.0f,  0.0f),

			Math::Vector3(0.0f, -1.0f,  0.0f),
			Math::Vector3(0.0f, -1.0f,  0.0f),
			Math::Vector3(0.0f, -1.0f,  0.0f),
			Math::Vector3(0.0f, -1.0f,  0.0f),
			Math::Vector3(0.0f, -1.0f,  0.0f),
			Math::Vector3(0.0f, -1.0f,  0.0f),

			Math::Vector3(0.0f,  1.0f,  0.0f),
			Math::Vector3(0.0f,  1.0f,  0.0f),
			Math::Vector3(0.0f,  1.0f,  0.0f),
			Math::Vector3(0.0f,  1.0f,  0.0f),
			Math::Vector3(0.0f,  1.0f,  0.0f),
			Math::Vector3(0.0f,  1.0f,  0.0f),
		};

		Cube::Cube()
		{

			std::vector<Vertex> vertices;


			Exceptions::NotImplementedException();

			for (int i = 0; i < Positions.size(); i++)
			{
				Vertex vert(Positions[i], Normals[i], UV[i]);
				vertices.push_back(vert);
			}
			MeshTexture _tex;
			API::Texture2D* Tex;
			_tex.tex = Tex;
			_tex.path = nullptr;
			_tex.type = "texture_diffuse";

			std::vector<MeshTexture> texturebuf;
			texturebuf.push_back(_tex);
			cube = new Mesh_NoIndices(vertices, texturebuf);

		}
		Cube::Cube(API::Texture2D * Diffuse)
		{
			
			std::vector<Vertex> vertices;
			
			Exceptions::NotImplementedException();

			for (int i = 0; i < Positions.size(); i++)
			{
				Vertex vert(Positions[i], Normals[i], UV[i]);
				vertices.push_back(vert);
			}
			MeshTexture _tex;
			_tex.tex = Diffuse;
			_tex.path = nullptr;
			_tex.type = "texture_diffuse";

			std::vector<MeshTexture> texturebuf;
			texturebuf.push_back(_tex);
			cube = new Mesh_NoIndices(vertices, texturebuf);
		}
		void Cube::Draw(API::Shader* shader)
		{
			cube->Draw(shader);
		}
	}
}