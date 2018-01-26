#include "AssimpImporter.h"
#include <AssetManager\AssetManager.h>
#pragma comment(lib,"assimp-vc140-mt.lib")

namespace NuclearEngine {
	namespace Internal {
		bool AssimpImporter::Load(std::string Path, Components::Model * _model)
		{
			this->model = _model;
			return LoadModel(Path);
		}

		bool AssimpImporter::LoadModel(std::string path)
		{
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

			//Failed?
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				Log->Error("[AssetManager] Failed to load model: " + path + "\nInfo: " + std::string(importer.GetErrorString()) + "\n");
				return false;
			}
			directory = path.substr(0, path.find_last_of('/'));
			ProcessNode(scene->mRootNode, scene);

			for (unsigned int i = 0; i < meshes_loaded.size(); i++)
			{
				model->Meshes.push_back(Components::Mesh(meshes_loaded.at(i)));
			}

			Log->Info("[AssetManager] Loaded Model: " + path + "\n");
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
		Components::MeshData AssimpImporter::ProcessMesh(aiMesh * mesh, const aiScene * scene)
		{
			Components::MeshData result;
			// Walk through each of the mesh's vertices
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				Components::Vertex vertex;
				Math::Vector3 vector;
				// positions
				vector = Math::Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
				vertex.Position = vector;
				// normals
				vector = Math::Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
				vertex.Normal = vector;
				// texture coordinates
				if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
				{
					// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
					// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
					vertex.TexCoords = Math::Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
				}
				else
				{
					vertex.TexCoords = Math::Vector2(0.0f, 0.0f);
				}

				result.vertices.push_back(vertex);
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
			std::vector<Imported_Texture> diffuseMaps = ProcessMaterialTexture(material, aiTextureType_DIFFUSE);
			std::vector<Imported_Texture> specularMaps = ProcessMaterialTexture(material, aiTextureType_SPECULAR);
					

			std::vector<Components::MeshTexture> DiffuseMaps = Imported2MeshTexture(diffuseMaps);
			std::vector<Components::MeshTexture> SpecularMaps = Imported2MeshTexture(specularMaps);

			result.textures.insert(result.textures.end(), DiffuseMaps.begin(), DiffuseMaps.end());
			result.textures.insert(result.textures.end(), SpecularMaps.begin(), SpecularMaps.end());

			// return a mesh object created from the extracted mesh data
			return result;
		}
		std::vector<Components::MeshTexture> AssimpImporter::Imported2MeshTexture(std::vector<Imported_Texture> textures)
		{
			std::vector<Components::MeshTexture> result;
			for (size_t i = 0; i < textures.size(); i++)
			{
				result.push_back(textures.at(i).Texture);
			}
			return result;
		}
		Components::MeshTextureType GetMeshTextureType(aiTextureType type)
		{
			switch (type)
			{
			case aiTextureType_DIFFUSE:
				return Components::MeshTextureType::Diffuse;
			case aiTextureType_SPECULAR:
				return Components::MeshTextureType::Specular;
			}
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

					API::Texture_Desc Desc;
					Desc.Filter = API::TextureFilter::Trilinear;
					Desc.Wrap = API::TextureWrap::Repeat;
					Desc.Format = API::Format::R8G8B8A8;
					Desc.Type = API::TextureType::Texture2D;

					std::string filename = texture.path;
					filename = directory + '/' + filename;
					AssetManager::CreateTextureFromFile(filename, &texture.Texture.Texture, Desc);

					textures.push_back(texture);
					textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.

				}
			}
			return textures;
		}
		
	}
}