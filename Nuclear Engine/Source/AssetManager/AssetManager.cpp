#include "AssetManager\AssetManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\Thirdparty\stb_image.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <Components\Model.h>
#pragma comment(lib,"assimp-vc140-mt.lib")

namespace NuclearEngine {
	struct ModelTexture {
		Components::MaterialTexture tex;
		std::string path;
	};
	class ModelImporter {
	public:
		 std::string directory;
		 Components::Model * model;
		 std::vector<ModelTexture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

		std::vector<Components::MaterialTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, Components::MaterialTextureType typeName)
		{
			std::vector<Components::MaterialTexture> textures;
			for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString str;
				mat->GetTexture(type, i, &str);
				// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
				bool skip = false;
				for (unsigned int j = 0; j < textures_loaded.size(); j++)
				{
					if (std::strcmp(textures_loaded[j].path.c_str(), str.C_Str()) == 0)
					{
						textures.push_back(textures_loaded[j].tex);
						skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
						break;
					}
				}

				API::Texture_Desc Desc;
				Desc.Filter = API::TextureFilter::Trilinear;
				Desc.Wrap = API::TextureWrap::Repeat;
				Desc.Format = API::Format::R8G8B8A8;
				Desc.Type = API::TextureType::Texture2D;
				if (!skip)
				{   // if texture hasn't been loaded already, load it
					ModelTexture texture;
					std::string filename = std::string(str.C_Str());
					filename = directory + '/' + filename;
					AssetManager::CreateTextureFromFile(filename, texture.tex.tex, Desc);
					texture.tex.type = typeName;
					texture.path = std::string(str.C_Str());
					textures.push_back(texture.tex);
					textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
				}
			}
			return textures;
		}
		Components::Mesh processMesh(aiMesh *mesh, const aiScene *scene)
		{
			// data to fill
			std::vector<Components::Vertex> vertices;
			std::vector<unsigned int> indices;
			std::vector<Components::MaterialTexture> textures;

			// Walk through each of the mesh's vertices
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				Components::Vertex vertex;
				Math::Vector3 vector;
				//Positions
				vector = Math::Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
				vertex.Position = vector;
				//Normals
				vector = Math::Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
				vertex.Normal = vector;

				// texture coordinates
				if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
				{
					vertex.TexCoords = Math::Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
				}
				else
				{
					vertex.TexCoords = Math::Vector2(0.0f, 0.0f);
				}
			}
			// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				// retrieve all indices of the face and store them in the indices vector
				for (unsigned int j = 0; j < face.mNumIndices; j++)
				{
					indices.push_back(face.mIndices[j]);

				}
			}
			// process materials
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			// 1. diffuse maps
			std::vector<Components::MaterialTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, Components::MaterialTextureType::Diffuse);
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. specular maps
			std::vector<Components::MaterialTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, Components::MaterialTextureType::Specular);
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

			// return a mesh object created from the extracted mesh data
			return Components::Mesh(Components::MeshData(vertices, indices, textures));
		}
		void ProcessNode(aiNode *node, const aiScene *scene)
		{
			// process each mesh located at the current node
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				// the node object only contains indices to index the actual objects in the scene. 
				// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				model->meshes.push_back(processMesh(mesh, scene));
			}
			// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				ProcessNode(node->mChildren[i], scene);
			}

		}
		bool load(std::string Path, Components::Model * _model)
		{
			Log->Info("[AssetManager] Loading Model: " + Path + "\n");
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(Path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
			// check for errors
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
			{
				Log->Error("[AssetManager] Failed to load model Info: " + std::string(importer.GetErrorString()) + "\n");
				return false;
			}
			this->model = _model;
			// retrieve the directory path of the filepath
			directory = Path.substr(0, Path.find_last_of('/'));

			// process ASSIMP's root node recursively
			ProcessNode(scene->mRootNode, scene);
		}
	};

	bool AssetManager::LoadModel(std::string Path, Components::Model * model)
	{
		ModelImporter importer;
		
		return importer.load(Path, model);
	}
	API::Texture_Data AssetManager::LoadTextureFromFile(std::string Path, const API::Texture_Desc & Desc)
	{
		
		int req_c;
		switch (Desc.Format)   //Correctly Set The Required Componenets for stb_image
		{
		case API::Format::R8:
			req_c = 1;
		case API::Format::R8G8:
			req_c = 2;
		case API::Format::R8G8B8:
			req_c = 3;
		case API::Format::R8G8B8A8:
			req_c = 4;
		default:
			req_c = 4;
		}

		API::Texture_Data Data;

		stbi_set_flip_vertically_on_load(true); // Note:FLip Texture in Y - axis while loading
		Data.Img_Data_Buf = stbi_load(Path.c_str(), &Data.Width, &Data.Height, &Data.Components_Number, req_c);

		if (Data.Img_Data_Buf == NULL)
		{
			Log->Error(std::string("[AssetManager] Failed To Load Texture: " + Path + '\n'));

		}

		Log->Info(std::string("[AssetManager] Loaded Texture: " + Path + '\n'));

		return Data;
	}

	void AssetManager::CreateTextureFromFile(std::string Path, API::Texture * texture, const API::Texture_Desc & Desc)
	{
		API::Texture::Create(texture, &AssetManager::LoadTextureFromFile(Path, Desc), Desc);
	}

	API::Texture_Data LoadTextureFromFile_NoFlip(std::string Path, const API::Texture_Desc & Desc)
	{
		

		int req_c;
		switch (Desc.Format)   //Correctly Set The Required Componenets for stb_image
		{
		case API::Format::R8:
			req_c = 1;
		case API::Format::R8G8:
			req_c = 2;
		case API::Format::R8G8B8:
			req_c = 3;
		case API::Format::R8G8B8A8:
			req_c = 4;
		default:
			req_c = 4;
		}

		API::Texture_Data Data;

		Data.Img_Data_Buf = stbi_load(Path.c_str(), &Data.Width, &Data.Height, &Data.Components_Number, req_c);
		if (Data.Img_Data_Buf == NULL)
		{
			Log->Error(std::string("[AssetManager] Failed To Load Texture: " + Path + '\n'));

		}

		Log->Info(std::string("[AssetManager] Loaded Texture: " + Path + '\n'));

		return Data;
	}
	std::array<API::Texture_Data, 6> AssetManager::LoadTextureCubeFromFile(const std::array<std::string, 6>& Paths, const API::Texture_Desc& Desc)
	{
		API::Texture_Data data1, data2, data3, data4, data5, data6;

		//URGENT FIX SKYBOX VERTICES
		data1 = LoadTextureFromFile_NoFlip(Paths.at(0), Desc);
		data2 = LoadTextureFromFile_NoFlip(Paths.at(1), Desc);
		data3 = LoadTextureFromFile_NoFlip(Paths.at(2), Desc);
		data4 = LoadTextureFromFile_NoFlip(Paths.at(3), Desc);
		data5 = LoadTextureFromFile_NoFlip(Paths.at(4), Desc);
		data6 = LoadTextureFromFile_NoFlip(Paths.at(5), Desc);

		std::array<API::Texture_Data, 6> result = { data1, data2, data3, data4, data5, data6 };

		return result;
	}
}