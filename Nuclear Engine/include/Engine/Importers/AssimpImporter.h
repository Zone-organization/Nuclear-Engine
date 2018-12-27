#pragma once

#ifdef EXPOSE_ASSIMP_IMPORTER
#include <Base\NE_Common.h>
#include <Engine\Assets\Mesh.h>
#include <Engine\Assets\Material.h>
#include <Assimp\include\assimp\Importer.hpp>
#include <Assimp\include\assimp\scene.h>
#include <Assimp\include\assimp\postprocess.h>
#include <Engine\Managers\AssetManager.h>

namespace NuclearEngine 
{
	namespace Importers
	{
		class AssimpImporter {
		public:
			std::tuple<Assets::Mesh, Assets::Material> Load(const std::string& Path, const Managers::MeshLoadingDesc& desc);

		private:
			std::vector<Assets::Texture> ProcessMaterialTexture(aiMaterial *mat, aiTextureType type);
			Assets::Mesh::SubMesh::SubMeshData ProcessMesh(aiMesh *mesh, const aiScene *scene);
			unsigned int ProcessMaterial(aiMesh * mesh, const aiScene * scene);
			void ProcessNode(aiNode *node, const aiScene *scene);

			std::vector<Assets::Mesh::SubMesh::SubMeshData> meshes_loaded;
			Assets::Material material;
			std::string directory;
			Assets::Mesh model;

			Managers::MeshLoadingDesc LoadingDesc;
		};


		std::tuple<Assets::Mesh, Assets::Material> AssimpLoadMesh(const std::string& Path, const Managers::MeshLoadingDesc& desc);

	}
}

#endif