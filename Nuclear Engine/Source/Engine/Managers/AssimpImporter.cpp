#include "AssimpImporter.h"
#include <Engine\Managers\AssetManager.h>
#include <Engine\Assets\Material.h>
#include <Base\Utilities\Hash.h>

#pragma comment(lib,"assimp.lib")

namespace NuclearEngine {
	namespace Internal {
		
		Assets::Mesh & AssimpImporter::Load(const std::string& Path, const Managers::MeshLoadingDesc& desc, Assets::Material* _material)
		{
			Log.Info("[AssetManager] Loading Mesh: " + Path + "\n");
			LoadingDesc = desc;
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(Path, aiProcess_Triangulate);

			//Failed?
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				Log.Error("[AssetManager] Assimp Failed to load mesh: " + Path + "\nInfo: " + std::string(importer.GetErrorString()) + "\n");
				return Assets::Mesh();
			}
			directory = Path.substr(0, Path.find_last_of('/'));
			ProcessNode(scene->mRootNode, scene);

			for (unsigned int i = 0; i < meshes_loaded.size(); i++)
			{
				model.SubMeshes.push_back(Assets::Mesh::SubMesh(meshes_loaded.at(i)));
			}
			auto hashedname = Utilities::Hash(Path);
			Log.Info("[AssetManager] Loaded Mesh: " + Path + "\n");

			_material = &(Managers::AssetManager::mImportedMaterials[hashedname] = material);
			return 	Managers::AssetManager::mImportedMeshes[hashedname] = model;

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
		void AssimpImporter::ProcessMaterial(aiMesh * mesh, const aiScene * scene)
		{	
			Assets::TextureSet TexSet;

			// process materials
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			if (LoadingDesc.LoadDiffuseTextures)
			{
				std::vector<Assets::Texture> DiffuseMaps = ProcessMaterialTexture(material, aiTextureType_DIFFUSE);
				TexSet.insert(TexSet.end(), DiffuseMaps.begin(), DiffuseMaps.end());
			}
			if (LoadingDesc.LoadSpecularTextures)
			{
				std::vector<Assets::Texture> SpecularMaps = ProcessMaterialTexture(material, aiTextureType_SPECULAR);
				TexSet.insert(TexSet.end(), SpecularMaps.begin(), SpecularMaps.end());
			}
			if (LoadingDesc.LoadNormalTextures)
			{
				std::vector<Assets::Texture> NormalMaps = ProcessMaterialTexture(material, aiTextureType_DISPLACEMENT);
				TexSet.insert(TexSet.end(), NormalMaps.begin(), NormalMaps.end());
			}

			this->material.mPixelShaderTextures.push_back(TexSet);
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
				if (LoadingDesc.UseNormals == true)
				{
					if (mesh->mNormals != NULL)
						result.Normals.push_back(Math::Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));			
					else	
						result.Normals.push_back(Math::Vector3(0.0f,0.0f,0.0f));
				}

				//tangents
				if (LoadingDesc.UseTangents == true)
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

			// process material		
			ProcessMaterial(mesh, scene);

			// return a mesh object created from the extracted mesh data
			return result;
		}
	
		Assets::TextureUsageType GetTextureType(aiTextureType type)
		{
			switch (type)
			{
			case aiTextureType_DIFFUSE:
				return Assets::TextureUsageType::Diffuse;
			case aiTextureType_SPECULAR:
				return Assets::TextureUsageType::Specular;
			case aiTextureType_DISPLACEMENT:
				return Assets::TextureUsageType::Normal;
			}

			//Unsupported types treated as diffuse
			return Assets::TextureUsageType::Diffuse;
		}
		std::vector<Assets::Texture> AssimpImporter::ProcessMaterialTexture(aiMaterial * mat, aiTextureType type)
		{
			std::vector<Assets::Texture> textures;
			for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString str;
				mat->GetTexture(type, i, &str);

				Assets::Texture texture;

				std::string filename = str.C_Str();
				filename = directory + '/' + filename;

				texture = Managers::AssetManager::Import(filename, GetTextureType(type));

				textures.push_back(texture);
			}
			return textures;
		}
		
	}
}