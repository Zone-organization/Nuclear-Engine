#pragma once

#ifdef EXPOSE_ASSIMP_IMPORTER
#include <Base\NE_Common.h>
#include <Engine\Assets\Mesh.h>
#include <Engine\Assets\Material.h>
#include <Engine/Importers/Common.h>
#include <Assimp\include\assimp\Importer.hpp>
#include <Assimp\include\assimp\scene.h>
#include <Assimp\include\assimp\postprocess.h>

namespace NuclearEngine 
{
	namespace Importers
	{
		class AssimpImporter {
		public:
			std::tuple<std::vector<Assets::Mesh::SubMesh>, Assets::Material> Load(const MeshImporterDesc& desc);

		private:
			Assets::TextureSet ProcessMaterialTexture(aiMaterial *mat, aiTextureType type);
			Assets::Mesh::SubMesh::SubMeshData ProcessMesh(aiMesh *mesh, const aiScene *scene);
			unsigned int ProcessMaterial(aiMesh * mesh, const aiScene * scene);
			void ProcessNode(aiNode *node, const aiScene *scene);

			std::vector<Assets::Mesh::SubMesh::SubMeshData> mMeshesLoaded;
			std::vector<std::string> TexturePaths;

			Assets::Material mMaterial;
			std::string mDirectory;
			std::vector<Assets::Mesh::SubMesh> mMesh;

			Importers::MeshLoadingDesc mLoadingDesc;
			Managers::AssetManager* mManager = nullptr;
		};


		std::tuple<std::vector<Assets::Mesh::SubMesh>, Assets::Material> AssimpLoadMesh(const MeshImporterDesc& desc);

	}
}

#endif