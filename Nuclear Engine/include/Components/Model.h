#pragma once
#include <Components\Mesh.h>
#include <string>
#include <vector>

struct aiNode;
struct aiScene;
struct aiMaterial;
enum aiTextureType;
struct aiMesh;

namespace NuclearEngine {
	namespace Components {

		struct Model_MaterialTex
		{
			MaterialTexture tex;
			const char* path;
		};

		class NEAPI Model
		{
		public:
			/*  Model Data */
			std::vector<Model_MaterialTex> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
			std::vector<Mesh> meshes;
			std::string directory;

			Model();

			void loadModel(const std::string& path, InputSignatures signature);

			virtual void Draw(API::Shader* shader);

		private:
			API::Shader* m_shader;

			// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
			void processNode(aiNode *node, const aiScene *scene);

			Mesh processMesh(aiMesh *mesh, const aiScene *scene);

			InputSignatures m_signature;

			// checks all material textures of a given type and loads the textures if they're not loaded yet.
			// the required info is returned as a Texture struct.
			std::vector<Model_MaterialTex> loadMaterialTextures(aiMaterial *mat, aiTextureType type, MaterialTextureType typeName);
		};

	}
}