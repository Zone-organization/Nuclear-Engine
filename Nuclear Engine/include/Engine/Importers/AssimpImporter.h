#pragma once

#ifdef EXPOSE_ASSIMP_IMPORTER
#include <Base\NE_Common.h>
#include <Engine\Assets\Mesh.h>
#include <Engine\Assets\Material.h>
#include <Engine/Importers/Common.h>
#include <Assimp\include\assimp\Importer.hpp>
#include <Assimp\include\assimp\scene.h>
#include <Assimp\include\assimp\postprocess.h>
#include "Engine\Animations\Bone.h"

namespace NuclearEngine 
{
	namespace Importers
	{
		class AssimpImporter {
		public:
			bool Load(const MeshImporterDesc& desc, Assets::Mesh* mesh, Assets::Material* material, Assets::Animation* anim);

		private:
			Assets::TextureSet ProcessMaterialTexture(aiMaterial *mat, aiTextureType type);
			void ProcessMesh(aiMesh *mesh, const aiScene *scene);
			unsigned int ProcessMaterial(aiMesh * mesh, const aiScene * scene);
			void ProcessNode(aiNode *node, const aiScene *scene);

			//std::vector<Assets::Mesh::SubMesh::SubMeshData> mMeshesLoaded;
			std::vector<std::string> TexturePaths;

			Assets::Material* mMaterial = nullptr;
			Assets::Animation* mAnimation = nullptr;
			Assets::Mesh* mMesh = nullptr;
			const aiScene* scene;
			std::string mDirectory;
			//std::vector<Assets::Mesh::SubMesh> mMesh;
			void ExtractBoneWeightForVertices(Assets::Mesh::SubMesh::SubMeshData* meshdata, aiMesh* mesh, const aiScene* scene);
			void ReadMissingBones(const aiAnimation* animation);
			void ReadHeirarchyData(Assets::AnimationNodeData& dest, const aiNode* src);

			Animations::BoneData InitBoneData(const aiNodeAnim* channel);
			Importers::MeshLoadingDesc mLoadingDesc;
			Managers::AssetManager* mManager = nullptr;
		};

		bool AssimpLoadMesh(const MeshImporterDesc& desc, Assets::Mesh* mesh, Assets::Material* material, Assets::Animation* anim);
	}
}

#endif