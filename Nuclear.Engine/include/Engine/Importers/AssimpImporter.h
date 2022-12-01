#pragma once

#include <Core\NE_Common.h>
#include <Engine\Assets\Mesh.h>
#include <Engine\Assets\Material.h>
#include <Engine/Importers/Common.h>
#include "Engine\Animation\Bone.h"
#include <Engine/Assets/Animations.h>

struct aiMaterial;
struct aiMesh;
struct aiScene;
struct aiNode;
struct aiAnimation;
struct aiNodeAnim;
namespace Assimp {
	class Importer;
}
namespace Nuclear 
{
	namespace Importers
	{
		class NEAPI AssimpImporter
		{
		public:
			AssimpImporter();
			~AssimpImporter();

			bool Load(const MeshImporterDesc& desc, Assets::Mesh* mesh, Assets::MaterialData* material, Assets::Animations* anim);
			bool IsExtensionSupported(const std::string& extension);
			Assimp::Importer* GetImporter();
		private:
			Assimp::Importer* pImporter;
		};


		class NEAPI AssimpLoader
		{
			Assets::TextureSet ProcessMaterialTexture(aiMaterial* mat, int type);
			void ProcessMesh(aiMesh* mesh, const aiScene* scene);
			Uint32 ProcessMaterial(aiMesh* mesh, const aiScene* scene);
			void ProcessNode(aiNode* node, const aiScene* scene);

		protected:
			friend class AssimpImporter;
			std::vector<std::string> TexturePaths;

			Assets::MaterialData* pMaterialData = nullptr;
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


		//bool AssimpLoadMesh(const MeshImporterDesc& desc, Assets::Mesh* mesh, Assets::MaterialData* material, Assets::Animations* anim);
	}
}
