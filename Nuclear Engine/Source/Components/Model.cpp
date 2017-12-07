#include "Components\Model.h"
#include <ResourceManager/ResourceManager.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace std;
namespace NuclearEngine {

	namespace Components {
		/*  Functions   */
		// constructor, expects a filepath to a 3D model.

		Model::Model() 
		{
						
			//loadModel(path);
		}

		// draws the model, and thus all its meshes

		void Model::Draw(API::Shader* shader)
		{
			for (unsigned int i = 0; i < meshes.size(); i++)
			{
				meshes[i].Draw(shader);
			}
		}

		/*  Functions   */
		// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.

		void Model::loadModel(const string & path, InputSignatures signature)
		{
			this->m_signature = signature;

			// read file via ASSIMP
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
			// check for errors
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
			{
				Log->Info(("[Model] Failed To Load: " + path + "\n Reason: " + importer.GetErrorString() + '\n'));
				return;
			}
			// retrieve the directory path of the filepath
			directory = path.substr(0, path.find_last_of('/'));

			// process ASSIMP's root node recursively
			processNode(scene->mRootNode, scene);

			Log->Info(("[Model] Loaded: " + path + '\n'));
		}

		// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).

		void Model::processNode(aiNode * node, const aiScene * scene)
		{
			// process each mesh located at the current node
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				std::cout << i;
				// the node object only contains indices to index the actual objects in the scene. 
				// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				meshes.push_back(processMesh(mesh, scene));
			}
			// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				processNode(node->mChildren[i], scene);
			}

		}
	

		Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene)
		{
			// data to fill
			vector<float> vertices;
			vector<unsigned int> indices;
			vector<Model_MaterialTex> textures;

			// Walk through each of the mesh's vertices
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{

				vertices.push_back(mesh->mVertices[i].x);
				vertices.push_back(mesh->mVertices[i].y);
				vertices.push_back(mesh->mVertices[i].z);

				if (m_signature == InputSignatures::Position_Normal_Texcoord)
				{	
					// normals
					vertices.push_back(mesh->mNormals[i].x);
					vertices.push_back(mesh->mNormals[i].y);
					vertices.push_back(mesh->mNormals[i].z);

					// texture coordinates
					if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
					{
						Math::Vector2 vec;
						// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
						// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
						vertices.push_back(mesh->mTextureCoords[0][i].x);
						vertices.push_back(mesh->mTextureCoords[0][i].y);
					}
					else
					{
						vertices.push_back(0);
						vertices.push_back(0);
					}
				}
				else if (m_signature == InputSignatures::Position_Texcoord)
				{
					// texture coordinates
					if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
					{
						Math::Vector2 vec;
						// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
						// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
						vertices.push_back(mesh->mTextureCoords[0][i].x);
						vertices.push_back(mesh->mTextureCoords[0][i].y);
					}
					else
					{
						vertices.push_back(0);
						vertices.push_back(0);
					}
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
			// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
			// Same applies to other texture as the following list summarizes:
			// diffuse: texture_diffuseN
			// specular: texture_specularN
			// normal: texture_normalN

			// 1. diffuse maps
			vector<Model_MaterialTex> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, MaterialTextureType::Diffuse);
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. specular maps
			vector<Model_MaterialTex> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, MaterialTextureType::Specular);
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

			vector<MaterialTexture> textures_;

			for (int i = 0; i < textures.size(); i++)
			{
				textures_.push_back(textures.at(i).tex);
			}

			// return a mesh object created from the extracted mesh data
			return Mesh(vertices, indices, textures_, m_signature);
		}

		// checks all material textures of a given type and loads the textures if they're not loaded yet.
		// the required info is returned as a Texture struct.
		vector<Model_MaterialTex> Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, MaterialTextureType typeName)
		{
			vector<Model_MaterialTex> textures;
			for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString str;
				mat->GetTexture(type, i, &str);
				// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
				bool skip = false;
				for (unsigned int j = 0; j < textures_loaded.size(); j++)
				{
					if (std::strcmp(textures_loaded[j].path, str.C_Str()) == 0)
					{
						textures.push_back(textures_loaded[j]);
						skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
						break;
					}
				}
				if (!skip)

				{   // if texture hasn't been loaded already, load it
					string filename = string(str.C_Str());
					filename = directory + '/' + filename;


					Texture_Desc Desc;
					Desc.Filter = TextureFilter::Trilinear;
					Desc.Wrap = TextureWrap::Repeat;
					Desc.Format = TextureFormat::R8G8B8A8;
				
					Model_MaterialTex texture;
					texture.tex.tex = new API::Texture(ResourceManager::LoadTextureFromFile(filename.c_str(), Desc), Desc);
					texture.tex.type = typeName;
					texture.path = str.C_Str();
					textures.push_back(texture);
					textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
				}
			}
			return textures;
		}
	}
}