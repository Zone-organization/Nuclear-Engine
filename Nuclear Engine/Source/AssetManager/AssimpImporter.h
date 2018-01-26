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
			Components::MeshTexture Texture;
			std::string path;
		};
		class AssimpImporter {
		public:
			bool Load(std::string Path, Components::Model* model);
		private:
			bool LoadModel(std::string Path);
			void ProcessNode(aiNode *node, const aiScene *scene);
			Components::MeshData ProcessMesh(aiMesh *mesh, const aiScene *scene);
			std::vector<Imported_Texture> ProcessMaterialTexture(aiMaterial *mat, aiTextureType type);
			std::vector<Components::MeshTexture> Imported2MeshTexture(std::vector<Imported_Texture> textures);

			Components::Model* model;
			std::vector<Imported_Texture> textures_loaded;
			std::vector<Components::MeshData> meshes_loaded;
			std::string directory;
		};
	}
}