#include "Components\Model.h"
#include <ResourceManager/ResourceManager.h>
#include <ResourceManager\ModelLoader\OBJ_Loader.h>
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

		Model::Model(string const & path, API::Shader* shader, bool gamma) : gammaCorrection(gamma)
		{
			this->m_shader = shader;
			//ModelLoader::OBJ_Loader loader;
			//bool res = loader.LoadFile(path);

			//cout << res << endl;;
			//// Go through each loaded mesh and out its contents
			//for (int i = 0; i < loader.LoadedMeshes.size(); i++)
			//{
			//	// Copy one of the loaded meshes to be our current mesh
			//	ModelLoader::Mesh curMesh = loader.LoadedMeshes[i];


			//	// Print Mesh Name
			//	cout << "Mesh " << i << ": " << curMesh.MeshName << "\n";

			//	// Print Vertices
			//	cout << "Vertices:\n";

			//	// Go through each vertex and print its number,
			//	//  position, normal, and texture coordinate
			//	for (int j = 0; j < curMesh.Vertices.size(); j++)
			//	{
			//		cout << "V" << j << ": " <<
			//			"P(" << curMesh.Vertices[j].Position.X << ", " << curMesh.Vertices[j].Position.Y << ", " << curMesh.Vertices[j].Position.Z << ") " <<
			//			"N(" << curMesh.Vertices[j].Normal.X << ", " << curMesh.Vertices[j].Normal.Y << ", " << curMesh.Vertices[j].Normal.Z << ") " <<
			//			"TC(" << curMesh.Vertices[j].TextureCoordinate.X << ", " << curMesh.Vertices[j].TextureCoordinate.Y << ")\n";
			//	}

			//	// Print Indices
			//	cout << "Indices:\n";

			//	// Go through every 3rd index and print the
			//	//	triangle that these indices represent
			//	for (int j = 0; j < curMesh.Indices.size(); j += 3)
			//	{
			//		cout << "T" << j / 3 << ": " << curMesh.Indices[j] << ", " << curMesh.Indices[j + 1] << ", " << curMesh.Indices[j + 2] << "\n";
			//	}

			//	// Print Material
			//	cout << "Material: " << curMesh.MeshMaterial.name << "\n";
			//	cout << "Ambient Color: " << curMesh.MeshMaterial.Ka.X << ", " << curMesh.MeshMaterial.Ka.Y << ", " << curMesh.MeshMaterial.Ka.Z << "\n";
			//	cout << "Diffuse Color: " << curMesh.MeshMaterial.Kd.X << ", " << curMesh.MeshMaterial.Kd.Y << ", " << curMesh.MeshMaterial.Kd.Z << "\n";
			//	cout << "Specular Color: " << curMesh.MeshMaterial.Ks.X << ", " << curMesh.MeshMaterial.Ks.Y << ", " << curMesh.MeshMaterial.Ks.Z << "\n";
			//	cout << "Specular Exponent: " << curMesh.MeshMaterial.Ns << "\n";
			//	cout << "Optical Density: " << curMesh.MeshMaterial.Ni << "\n";
			//	cout << "Dissolve: " << curMesh.MeshMaterial.d << "\n";
			//	cout << "Illumination: " << curMesh.MeshMaterial.illum << "\n";
			//	cout << "Ambient Texture Map: " << curMesh.MeshMaterial.map_Ka << "\n";
			//	cout << "Diffuse Texture Map: " << curMesh.MeshMaterial.map_Kd << "\n";
			//	cout << "Specular Texture Map: " << curMesh.MeshMaterial.map_Ks << "\n";
			//	cout << "Alpha Texture Map: " << curMesh.MeshMaterial.map_d << "\n";
			//	file << "Bump Map: " << curMesh.MeshMaterial.map_bump << "\n";

			//	// Leave a space to separate from the next mesh
			//	file << "\n";
			//}
			
			loadModel(path);
		}

		// draws the model, and thus all its meshes

		void Model::Draw()
		{
			for (unsigned int i = 0; i < meshes.size(); i++)
				meshes[i].Draw();
		}

		/*  Functions   */
		// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.

		void Model::loadModel(string const & path)
		{
			cout << "Loading: " << path << endl;

			// read file via ASSIMP
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
			// check for errors
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
			{
				cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
				return;
			}
			// retrieve the directory path of the filepath
			directory = path.substr(0, path.find_last_of('/'));

			// process ASSIMP's root node recursively
			processNode(scene->mRootNode, scene);
		}

		// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).

		void Model::processNode(aiNode * node, const aiScene * scene)
		{
			// process each mesh located at the current node
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
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
			vector<Vertex> vertices;
			vector<unsigned int> indices;
			vector<MeshTexture> textures;

			// Walk through each of the mesh's vertices
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				Vertex vertex;
				Math::Vector3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
								  // positions
				vector.x = mesh->mVertices[i].x;
				vector.y = mesh->mVertices[i].y;
				vector.z = mesh->mVertices[i].z;
				vertex.Position = vector;
				// normals
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.Normal = vector;
				// texture coordinates
				if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
				{
					Math::Vector2 vec;
					// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
					// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
					vec.x = mesh->mTextureCoords[0][i].x;
					vec.y = mesh->mTextureCoords[0][i].y;
					vertex.TexCoords = vec;
				}
				else
					vertex.TexCoords = Math::Vector2(0.0f, 0.0f);
				// tangent
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.Tangent = vector;
				// bitangent
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.Bitangent = vector;
				vertices.push_back(vertex);
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
			vector<MeshTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. specular maps
			vector<MeshTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			//// 3. normal maps
			std::vector<MeshTexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
			//// 4. height maps
			std::vector<MeshTexture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
			textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

			// return a mesh object created from the extracted mesh data
			return Mesh(m_shader, vertices, indices, textures);
		}

		// checks all material textures of a given type and loads the textures if they're not loaded yet.
		// the required info is returned as a Texture struct.

		vector<MeshTexture> Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, string typeName)
		{
			vector<MeshTexture> textures;
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
				
					MeshTexture texture;
					texture.tex = new API::Texture2D(ResourceManager::LoadTextureFromFile(filename.c_str(), Desc), Desc);
					texture.type = typeName.c_str();
					texture.path = str.C_Str();
					textures.push_back(texture);
					textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
				}
			}
			return textures;
		}
	}
}