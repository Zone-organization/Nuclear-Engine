#include "AssimpImporter.h"
#include <Managers\AssetManager.h>
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
			Log.Info("[AssetManager] Loading Model: " + path + "\n");
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

			//Failed?
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				Log.Error("[AssetManager] Assimp Failed to load model: " + path + "\nInfo: " + std::string(importer.GetErrorString()) + "\n");
				return false;
			}
			directory = path.substr(0, path.find_last_of('/'));
			ProcessNode(scene->mRootNode, scene);

			for (unsigned int i = 0; i < meshes_loaded.size(); i++)
			{
				model->SubMeshes.push_back(Assets::Mesh(meshes_loaded.at(i)));
			}

			Log.Info("[AssetManager] Loaded Model: " + path + "\n");
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
				std::vector<Imported_Texture> diffuseMaps = ProcessMaterialTexture(material, aiTextureType_DIFFUSE);
				std::vector<Assets::MeshTexture> DiffuseMaps = Imported2MeshTexture(diffuseMaps);
				result.textures.insert(result.textures.end(), DiffuseMaps.begin(), DiffuseMaps.end());
			}
			if (loaddesc.LoadSpecularTextures)
			{
				std::vector<Imported_Texture> specularMaps = ProcessMaterialTexture(material, aiTextureType_SPECULAR);
				std::vector<Assets::MeshTexture> SpecularMaps = Imported2MeshTexture(specularMaps);
				result.textures.insert(result.textures.end(), SpecularMaps.begin(), SpecularMaps.end());
			}
			if (loaddesc.LoadNormalTextures)
			{
				std::vector<Imported_Texture> normalMaps = ProcessMaterialTexture(material, aiTextureType_DISPLACEMENT);
				std::vector<Assets::MeshTexture> NormalMaps = Imported2MeshTexture(normalMaps);
				result.textures.insert(result.textures.end(), NormalMaps.begin(), NormalMaps.end());
			}
			// return a mesh object created from the extracted mesh data
			return result;
		}
		std::vector<Assets::MeshTexture> AssimpImporter::Imported2MeshTexture(std::vector<Imported_Texture> textures)
		{
			std::vector<Assets::MeshTexture> result;
			for (size_t i = 0; i < textures.size(); i++)
			{
				result.push_back(textures.at(i).Texture);
			}
			return result;
		}
		Assets::MeshTextureType GetMeshTextureType(aiTextureType type)
		{
			switch (type)
			{
			case aiTextureType_DIFFUSE:
				return Assets::MeshTextureType::Diffuse;
			case aiTextureType_SPECULAR:
				return Assets::MeshTextureType::Specular;
			case aiTextureType_DISPLACEMENT:
				return Assets::MeshTextureType::Normal;
			}

			//Unsupported types treated as diffuse
			return Assets::MeshTextureType::Diffuse;
		}
		std::vector<Imported_Texture> AssimpImporter::ProcessMaterialTexture(aiMaterial * mat, aiTextureType type)
		{
			std::vector<Imported_Texture> textures;
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
						textures.push_back(textures_loaded[j]);
						skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
						break;
					}
				}
				if (!skip)
				{
					// if texture hasn't been loaded already, load it
					Imported_Texture texture;
					texture.path = str.C_Str();
					texture.Texture.type = GetMeshTextureType(type);

					Graphics::API::Texture_Desc Desc;
					//Desc.Filter = Graphics::API::TextureFilter::Trilinear;
					//Desc.Wrap = Graphics::API::TextureWrap::Repeat;
					Desc.Format = Graphics::API::Format::R8G8B8A8_UNORM;
					Desc.Type = Graphics::API::TextureType::Texture2D;

					std::string filename = texture.path;
					filename = directory + '/' + filename;
					Managers::AssetManager::CreateTextureFromFile(filename, &texture.Texture.Texture, Desc);

					textures.push_back(texture);
					textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.

				}
			}
			return textures;
		}
		
	}
}