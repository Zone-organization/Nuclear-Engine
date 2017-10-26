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
		class NEAPI Model
		{
		public:
			/*  Model Data */
			std::vector<MeshTexture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
			std::vector<Mesh> meshes;
			std::string directory;
			bool gammaCorrection;

			/*  Functions   */
			// constructor, expects a filepath to a 3D model.
			Model(std::string const &path, API::Shader* shader, bool gamma = false);

			// draws the model, and thus all its meshes
			void Draw();

		private:

			API::Shader* m_shader;
			/*  Functions   */
			// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
			void loadModel(std::string const &path);

			// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
			void processNode(aiNode *node, const aiScene *scene);

			Mesh processMesh(aiMesh *mesh, const aiScene *scene);

			// checks all material textures of a given type and loads the textures if they're not loaded yet.
			// the required info is returned as a Texture struct.
			std::vector<MeshTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
		};

	}
}