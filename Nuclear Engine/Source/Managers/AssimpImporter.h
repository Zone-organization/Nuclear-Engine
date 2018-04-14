#pragma once
#include <NE_Common.h>
#include <Assets\Mesh.h>
#include <Assimp\include\assimp\Importer.hpp>
#include <Assimp\include\assimp\scene.h>
#include <Assimp\include\assimp\postprocess.h>
#include <Managers\AssetManager.h>
namespace NuclearEngine {
	namespace Internal {
		struct Imported_Texture
		{
			Assets::MeshTexture Texture;
			std::string path;
		};
		class AssimpImporter {
		public:
			bool Load(std::string Path, Assets::Mesh* model, const Managers::MeshLoadingDesc& desc);
		private:
			bool LoadModel(std::string Path);
			void ProcessNode(aiNode *node, const aiScene *scene);
			Assets::Mesh::SubMesh::SubMeshData ProcessMesh(aiMesh *mesh, const aiScene *scene);
			std::vector<Imported_Texture> ProcessMaterialTexture(aiMaterial *mat, aiTextureType type);
			std::vector<Assets::MeshTexture> Imported2MeshTexture(std::vector<Imported_Texture> textures);
			
			Assets::Mesh* model;
			std::vector<Imported_Texture> textures_loaded;
			std::vector<Assets::Mesh::SubMesh::SubMeshData> meshes_loaded;
			std::string directory;

			Managers::MeshLoadingDesc loaddesc;
		};
	}
}