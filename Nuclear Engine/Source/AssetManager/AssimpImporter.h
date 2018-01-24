#pragma once
#include <NE_Common.h>
#include <Components\Model.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

namespace NuclearEngine {
	namespace Internal {
		struct Imported_Texture
		{
			Components::MeshTextureType type;
			std::string path;
		};
		class AssimpImporter {
		public:
			bool Load(std::string Path, Components::Model* model);
		private:
			std::vector<Components::MeshTexture> LoadMeshTextures(std::vector<Imported_Texture> textures);
			bool LoadModel(std::string Path);
			void ProcessNode(aiNode *node, const aiScene *scene);
			Components::MeshData ProcessMesh(aiMesh *mesh, const aiScene *scene);
			std::vector<Imported_Texture> ProcessMaterialTexture(aiMaterial *mat, aiTextureType type);

			Components::Model* model;
			std::vector<Imported_Texture> textures_loaded;
			std::vector<Components::MeshData> meshes_loaded;
			std::string directory;
		};
	}
}