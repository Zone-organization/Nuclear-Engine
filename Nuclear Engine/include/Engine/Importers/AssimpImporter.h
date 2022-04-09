#pragma once

#ifdef EXPOSE_ASSIMP_IMPORTER
#include <Base\NE_Common.h>
#include <Engine\Assets\Mesh.h>
#include <Engine\Assets\Material.h>
#include <Engine/Importers/Common.h>
#include <Assimp\include\assimp\Importer.hpp>
#include <Assimp\include\assimp\scene.h>
#include <Assimp\include\assimp\postprocess.h>
#include "Engine\Animation\Bone.h"
#include <Engine/Assets/Mesh.h>
#include <Engine/Assets/Material.h>
#include <Engine/Assets/Animations.h>

namespace NuclearEngine 
{
	namespace Importers
	{
		class AssimpImporter {
		public:
			bool Load(const MeshImporterDesc& desc, Assets::Mesh* mesh, Assets::Material* material, Assets::Animations* anim);
		private:
			Assets::TextureSet ProcessMaterialTexture(aiMaterial *mat, const aiTextureType& type);
			void ProcessMesh(aiMesh *mesh, const aiScene *scene);
			Uint32 ProcessMaterial(aiMesh * mesh, const aiScene * scene);
			void ProcessNode(aiNode *node, const aiScene *scene);

			std::vector<std::string> TexturePaths;

			Assets::Material* mMaterial = nullptr;
			Assets::Animations* mAnimation = nullptr;
			Assets::Mesh* mMesh = nullptr;
			const aiScene* scene;
			std::string mDirectory;

			void LoadAnimations();
			void ExtractBoneWeightForVertices(Assets::Mesh::SubMesh::SubMeshData* meshdata, aiMesh* mesh, const aiScene* scene);
			void ReadMissingBones(const aiAnimation* animation, Animation::AnimationClip* clip);
			void ReadHeirarchyData(Animation::ClipNodeData* dest, const aiNode* src);

			void InitBoneData(const aiNodeAnim* channel, Animation::BoneData& data);
			Importers::MeshLoadingDesc mLoadingDesc;
			Managers::AssetManager* mManager = nullptr;
		};

		bool AssimpLoadMesh(const MeshImporterDesc& desc, Assets::Mesh* mesh, Assets::Material* material, Assets::Animations* anim);
	}
}

#endif