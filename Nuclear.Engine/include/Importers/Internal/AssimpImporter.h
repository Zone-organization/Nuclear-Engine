#pragma once
#include <NE_Common.h>
#include <Assets\Mesh.h>
#include <Assets\Material.h>
#include <Importers/Common.h>
#include "Animation\Bone.h"
#include <Assets/Animations.h>

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
		namespace Internal 
		{
			class NEAPI AssimpImporter
			{
			public:
				AssimpImporter();
				~AssimpImporter();

				bool Load(const MeshLoadingDesc& desc, const std::string& Path, Assets::Mesh* mesh, Assets::MaterialData* material, Assets::Animations* anim);
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
			};


			//bool AssimpLoadMesh(const MeshImporterDesc& desc, Assets::Mesh* mesh, Assets::MaterialData* material, Assets::Animations* anim);
		}
	}
}
