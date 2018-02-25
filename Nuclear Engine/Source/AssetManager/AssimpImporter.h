#pragma once
#include <NE_Common.h>
#include "AssetManager\ModelAsset.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <AssetManager\AssetManager.h>
namespace NuclearEngine {
	namespace Internal {
		struct Imported_Texture
		{
			Assets::MeshTexture Texture;
			std::string path;
		};
		class AssimpImporter {
		public:
			bool Load(std::string Path, Assets::ModelAsset* model, const ModelLoadingDesc& desc);
		private:
			bool LoadModel(std::string Path);
			void ProcessNode(aiNode *node, const aiScene *scene);
			Assets::MeshData ProcessMesh(aiMesh *mesh, const aiScene *scene);
			std::vector<Imported_Texture> ProcessMaterialTexture(aiMaterial *mat, aiTextureType type);
			std::vector<Assets::MeshTexture> Imported2MeshTexture(std::vector<Imported_Texture> textures);
			Assets::ModelAsset* model;
			std::vector<Imported_Texture> textures_loaded;
			std::vector<Assets::MeshData> meshes_loaded;
			std::string directory;

			ModelLoadingDesc loaddesc;
		};
	}
}