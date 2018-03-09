#pragma once
#include <NE_Common.h>
#include "XAsset\ModelAsset.h"
#include <Assimp\include\assimp\Importer.hpp>
#include <Assimp\include\assimp\scene.h>
#include <Assimp\include\assimp\postprocess.h>
#include <Managers\AssetManager.h>
namespace NuclearEngine {
	namespace Internal {
		struct Imported_Texture
		{
			XAsset::MeshTexture Texture;
			std::string path;
		};
		class AssimpImporter {
		public:
			bool Load(std::string Path, XAsset::ModelAsset* model, const Managers::ModelLoadingDesc& desc);
		private:
			bool LoadModel(std::string Path);
			void ProcessNode(aiNode *node, const aiScene *scene);
			XAsset::MeshData ProcessMesh(aiMesh *mesh, const aiScene *scene);
			std::vector<Imported_Texture> ProcessMaterialTexture(aiMaterial *mat, aiTextureType type);
			std::vector<XAsset::MeshTexture> Imported2MeshTexture(std::vector<Imported_Texture> textures);
			XAsset::ModelAsset* model;
			std::vector<Imported_Texture> textures_loaded;
			std::vector<XAsset::MeshData> meshes_loaded;
			std::string directory;

			Managers::ModelLoadingDesc loaddesc;
		};
	}
}