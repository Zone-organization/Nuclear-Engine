#include "AssimpImporter.h"
#include <Engine\Managers\AssetManager.h>
#pragma comment(lib,"assimp.lib")

namespace NuclearEngine {
	namespace Internal {
		bool AssimpImporter::Load(std::string Path, Assets::Mesh * _model, const Managers::MeshLoadingDesc& desc)
		{
			loaddesc = desc;
			this->model = _model;
			return LoadModel(Path);
		}

		bool AssimpImporter::LoadModel(std::string path)
		{
			Log.Info("[AssetManager] Loading Mesh: " + path + "\n");
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

			//Failed?
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				Log.Error("[AssetManager] Assimp Failed to load mesh: " + path + "\nInfo: " + std::string(importer.GetErrorString()) + "\n");
				return false;
			}
			directory = path.substr(0, path.find_last_of('/'));
			ProcessNode(scene->mRootNode, scene);

			for (unsigned int i = 0; i < meshes_loaded.size(); i++)
			{
				model->SubMeshes.push_back(Assets::Mesh::SubMesh(meshes_loaded.at(i)));
			}

			Log.Info("[AssetManager] Loaded Mesh: " + path + "\n");
			return true;
		}
		void AssimpImporter::ProcessNode(aiNode * node, const aiScene * scene)
		{
			// process each mesh located at the current node
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				// the node object only contains indices to index the actual objects in the scene. 
				// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				meshes_loaded.push_back(ProcessMesh(mesh, scene));
			}
			// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				ProcessNode(node->mChildren[i], scene);
			}
		}
		Assets::Mesh::SubMesh::SubMeshData AssimpImporter::ProcessMesh(aiMesh * mesh, const aiScene * scene)
		{
			Assets::Mesh::SubMesh::SubMeshData result;

			// Walk through each of the mesh's vertices
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				// positions
				result.Positions.push_back(Math::Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
				// texture coordinates
				if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
				{
					// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
					// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
					result.UV.push_back(Math::Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
				}
				else
				{
					result.UV.push_back(Math::Vector2(0.0f, 0.0f));
				}
				// normals
				if (loaddesc.UseNormals == true)
				{
					if (mesh->mNormals != NULL)
						result.Normals.push_back(Math::Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));			
					else	
						result.Normals.push_back(Math::Vector3(0.0f,0.0f,0.0f));
				}

				//tangents
				if (loaddesc.UseTangents == true)
				{
					if (mesh->mTangents != NULL)
						result.Tangents.push_back(Math::Vector3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z));
					else
						result.Tangents.push_back(Math::Vector3(0.0f, 0.0f, 0.0f));
				}
			}
			// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++)
				{
					result.indices.push_back(face.mIndices[j]);
				}
			}
			// process materials
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			if (loaddesc.LoadDiffuseTextures)
			{
				std::vector<Assets::Texture> DiffuseMaps = ProcessMaterialTexture(material, aiTextureType_DIFFUSE);
				result.textures.insert(result.textures.end(), DiffuseMaps.begin(), DiffuseMaps.end());
			}
			if (loaddesc.LoadSpecularTextures)
			{
				std::vector<Assets::Texture> SpecularMaps = ProcessMaterialTexture(material, aiTextureType_SPECULAR);
				result.textures.insert(result.textures.end(), SpecularMaps.begin(), SpecularMaps.end());
			}
			if (loaddesc.LoadNormalTextures)
			{
				std::vector<Assets::Texture> NormalMaps = ProcessMaterialTexture(material, aiTextureType_DISPLACEMENT);
				result.textures.insert(result.textures.end(), NormalMaps.begin(), NormalMaps.end());
			}
			// return a mesh object created from the extracted mesh data
			return result;
		}
	
		Assets::TextureType GetAssets::TextureType(aiTextureType type)
		{
			switch (type)
			{
			case aiTextureType_DIFFUSE:
				return Assets::TextureType::Diffuse;
			case aiTextureType_SPECULAR:
				return Assets::TextureType::Specular;
			case aiTextureType_DISPLACEMENT:
				return Assets::TextureType::Normal;
			}

			//Unsupported types treated as diffuse
			return Assets::TextureType::Diffuse;
		}
		std::vector<Assets::Texture> AssimpImporter::ProcessMaterialTexture(aiMaterial * mat, aiTextureType type)
		{
			std::vector<Assets::Texture> textures;
			for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString str;
				mat->GetTexture(type, i, &str);

				Assets::Texture texture;
				texture.type = GetAssets::TextureType(type);

				Graphics::API::Texture_Desc Desc;
				Desc.Format = Graphics::API::Format::R8G8B8A8_UNORM;
				Desc.Type = Graphics::API::TextureType::Texture2D;

				std::string filename = str.C_Str();
				filename = directory + '/' + filename;
				Managers::AssetManager::CreateTextureFromFile(filename, &texture.Texture, Desc);

				textures.push_back(texture);
			}
			return textures;
		}
		
	}
}