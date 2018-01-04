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
		std::vector<Math::Vector2> TexCoords = {
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

		Cube::Cube()
		{
			Exceptions::NotImplementedException();	
		}
		
		std::vector<float> ParseInputSignatureForCube(InputSignatures Signature)
		{
			std::vector<float> result;

			if (Signature == InputSignatures::Position)
			{
				for (int i = 0; i < 36; i++)
				{
					result.push_back(Positions[i].x);
					result.push_back(Positions[i].y);
					result.push_back(Positions[i].z);
				}
			}
			else if (Signature == InputSignatures::Position_Texcoord)
			{
				for (int i = 0; i < 36; i++)
				{
					result.push_back(Positions[i].x);
					result.push_back(Positions[i].y);
					result.push_back(Positions[i].z);
					result.push_back(TexCoords[i].x);
					result.push_back(TexCoords[i].y);
				}
			}
			else if (Signature == InputSignatures::Position_Normal_Texcoord)
			{
				for (int i = 0; i < 36; i++)
				{
					result.push_back(Positions[i].x);
					result.push_back(Positions[i].y);
					result.push_back(Positions[i].z);
					result.push_back(Normals[i].x);
					result.push_back(Normals[i].y);
					result.push_back(Normals[i].z);
					result.push_back(TexCoords[i].x);
					result.push_back(TexCoords[i].y);
				}
			}
			return result;
		}

		Cube::Cube(InputSignatures Signature, Shading::Material * mat)
		{
			std::vector<MaterialTexture> texturebuf;

			if (mat != nullptr)
			{
				MaterialTexture _tex;
				_tex.tex = mat->Diffuse;
				_tex.type = MaterialTextureType::Diffuse;

				texturebuf.push_back(_tex);

				if (mat->Specular != nullptr)
				{
					MaterialTexture __tex;
					__tex.tex = mat->Specular;
					__tex.type = MaterialTextureType::Specular;
					texturebuf.push_back(__tex);
				}

			}
			cube = new Mesh_NoIndices(ParseInputSignatureForCube(Signature), texturebuf, Signature);
		}
		void Cube::Draw(API::Shader* shader)
		{
			cube->Draw(shader);
		}
	}
}