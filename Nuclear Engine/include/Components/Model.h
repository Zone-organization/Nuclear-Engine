#pragma once
#include <Components\Mesh.h>
#include <string>
#include <vector>
//
//struct aiNode;
//struct aiScene;
//struct aiMaterial;
//enum aiTextureType;
//struct aiMesh;

namespace NuclearEngine {
	namespace Components {
		class NEAPI Model
		{
		public:
			/*  Model Data */
			std::vector<MaterialTexture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
			std::vector<Mesh> meshes;
			std::string directory;

			Model();

			virtual void Draw(API::Shader* shader);

		private:
			API::Shader* m_shader;
			/*  Functions   */
			// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
			//void loadModel(std::string const &path);

			// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
			//void processNode(aiNode *node, const aiScene *scene);

			//Mesh processMesh(aiMesh *mesh, const aiScene *scene);

			// checks all material textures of a given type and loads the textures if they're not loaded yet.
			// the required info is returned as a Texture struct.
			//std::vector<MaterialTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
		};

	}
}