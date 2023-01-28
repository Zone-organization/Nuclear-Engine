#include <Assets\Importers\AssimpImporter.h>
#include <Assets\AssetManager.h>
#include "Graphics\GraphicsEngine.h"
#include <Assets\Material.h>
#include "AssimpGLMHelpers.h"
#include <Utilities/Logger.h>
#include <Assimp\include\assimp\Exporter.hpp>
#include <Assimp\include\assimp\Importer.hpp>
#include <Assimp\include\assimp\scene.h>
#include <Assimp\include\assimp\postprocess.h>
#include <Assets\Mesh.h>
#include <Assets\Material.h>
#include <Assets/Animations.h>
#include "Animation\Bone.h"

namespace Nuclear {
	namespace Assets {
		namespace Importers
		{

			class AssimpLoader
			{
			public:
				Assets::MaterialTextureSet ProcessMaterialTexture(aiMaterial* mat, int type);
				void ProcessMesh(aiMesh* mesh, const aiScene* scene);
				Uint32 ProcessMaterial(aiMesh* mesh, const aiScene* scene);
				void ProcessNode(aiNode* node, const aiScene* scene);

			protected:
				friend class AssimpImporter;
				std::vector<std::string> TexturePaths;

				Model mModel;
				const aiScene* scene;
				std::string mDirectory;
				std::string exportPath;

				void LoadAnimations();
				void ExtractBoneWeightForVertices(Assets::Mesh::SubMesh::SubMeshData* meshdata, aiMesh* mesh, const aiScene* scene);
				void ReadMissingBones(const aiAnimation* animation, Animation::AnimationClip* clip);
				void ReadHeirarchyData(Animation::ClipNodeData* dest, const aiNode* src);

				void InitBoneData(const aiNodeAnim* channel, Animation::BoneData& data);
				MeshImportingDesc mLoadingDesc;
			};


			inline void SetVertexBoneData(Assets::Mesh::Vertex& vertex, int boneID, float weight)
			{
				vertex.Weights = Math::Vector4(weight);
				vertex.BoneIDs = Math::Vector4i(boneID);
			}

			AssimpImporter::AssimpImporter()
			{
				pImporter = new Assimp::Importer();
				pExporter = new Assimp::Exporter();
			}

			AssimpImporter::~AssimpImporter()
			{
				delete pImporter;
				delete pExporter;
			}

			bool AssimpImporter::isBusy()
			{
				return busy;
			}


			bool AssimpImporter::Import(const std::string& importPath, const std::string& exportPath, const Model& model, const Assets::MeshImportingDesc& desc)
			{
				busy = true;
				AssimpLoader loader;

				loader.mLoadingDesc = desc;
				loader.mModel = model;
				loader.scene = pImporter->ReadFile(importPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
				loader.exportPath = exportPath;
				//Failed?
				if (!loader.scene || loader.scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !loader.scene->mRootNode)
				{
					NUCLEAR_ERROR("[AssimpImporter] Assimp Failed to import mesh: '{0}' Info: '{1}'", importPath, pImporter->GetErrorString());
					return false;
				}
				loader.mDirectory = importPath.substr(0, importPath.find_last_of('/'));
				loader.ProcessNode(loader.scene->mRootNode, loader.scene);

				if (loader.mModel.pAnimations && loader.scene->mAnimations != nullptr)
				{
					loader.LoadAnimations();
					loader.mModel.pAnimations->SetState(Assets::IAsset::State::Loaded);
				}
				if (loader.mModel.pMesh)
				{
					loader.mModel.pMesh->SetState(Assets::IAsset::State::Loaded);
				}
		
				pExporter->Export(loader.scene, "glb2", loader.exportPath + model.mName + ".glb");

				busy = false;
				return true;
			}

			bool AssimpImporter::Load(const std::string& Path, const Model& model, const MeshImportingDesc& desc)
			{
				busy = true;
				AssimpLoader loader;

				loader.mLoadingDesc = desc;
				loader.mModel = model;

				loader.scene = pImporter->ReadFile(Path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

				//Failed?
				if (!loader.scene || loader.scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !loader.scene->mRootNode)
				{
					NUCLEAR_ERROR("[AssimpImporter] Assimp Failed to load mesh: '{0}' Info: '{1}'", Path, pImporter->GetErrorString());
					return false;
				}
				loader.mDirectory = Path.substr(0, Path.find_last_of('/'));
				loader.ProcessNode(loader.scene->mRootNode, loader.scene);

				if (loader.mModel.pAnimations && loader.scene->mAnimations != nullptr)
				{
					loader.LoadAnimations();
					loader.mModel.pAnimations->SetState(Assets::IAsset::State::Loaded);
				}
				if (loader.mModel.pMesh)
				{
					loader.mModel.pMesh->SetState(Assets::IAsset::State::Loaded);
				}

				busy = false;
				return true;
			}

			bool AssimpImporter::IsExtensionSupported(const std::string& extension)
			{
				return pImporter->IsExtensionSupported(extension);
			}

			Assimp::Importer* AssimpImporter::GetImporter()
			{
				return pImporter;
			}

			void AssimpLoader::ProcessNode(aiNode* node, const aiScene* scene)
			{
				// process each mesh located at the current node
				for (unsigned int i = 0; i < node->mNumMeshes; i++)
				{
					// the node object only contains indices to index the actual objects in the scene. 
					// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
					aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
					ProcessMesh(mesh, scene);
				}
				// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
				for (unsigned int i = 0; i < node->mNumChildren; i++)
				{
					ProcessNode(node->mChildren[i], scene);
				}
			}
			Uint32 AssimpLoader::ProcessMaterial(aiMesh* mesh, const aiScene* scene)
			{
				if (mModel.pMaterial)
				{
					auto& materialtextures = mModel.pMaterial->GetTextures();
					Assets::MaterialTextureSet TexSet;

					// process materials
					aiMaterial* MeshMat = scene->mMaterials[mesh->mMaterialIndex];
					auto MeshName = std::string(MeshMat->GetName().C_Str());
					TexSet.mHashedName = Utilities::Hash(MeshName);


					for (Uint32 i = 0; i < materialtextures.size(); i++)
					{
						if (TexSet.mHashedName == materialtextures.at(i).mHashedName)
						{
							return i;
						}
					}

					std::vector<aiTextureType> LoadTypes{
						aiTextureType_DIFFUSE,
						aiTextureType_SPECULAR,
						aiTextureType_AMBIENT,
						aiTextureType_EMISSIVE,
						aiTextureType_HEIGHT,
						aiTextureType_NORMALS ,
						aiTextureType_SHININESS,
						aiTextureType_OPACITY,
						aiTextureType_DISPLACEMENT,
						aiTextureType_LIGHTMAP,
						aiTextureType_REFLECTION,
						aiTextureType_UNKNOWN
					};
					for (auto i : LoadTypes)
					{
						auto Textures = ProcessMaterialTexture(MeshMat, i);
						TexSet.mData.insert(TexSet.mData.end(), Textures.mData.begin(), Textures.mData.end());
					}

					if (!TexSet.mData.empty())
					{
						//auto MeshName = std::string(MeshMat->GetName().C_Str());
						//TexSet.mHashedName = Utilities::Hash(MeshName);
						if (mLoadingDesc.SaveMaterialNames == true)
						{
							TexSet.mName = MeshName;
						}

						materialtextures.push_back(TexSet);

						return static_cast<Uint32>(materialtextures.size() - 1);
					}
				}
				return 0;
			}

			void AssimpLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene)
			{
				assert(mModel.pMesh);

				mModel.pMesh->mSubMeshes.push_back(Assets::Mesh::SubMesh::SubMeshData());

				Assets::Mesh::SubMesh::SubMeshData* result = &mModel.pMesh->mSubMeshes.back().data;
				result->Vertices.reserve(mesh->mNumVertices);


				// Walk through each of the mesh's vertices
				for (unsigned int i = 0; i < mesh->mNumVertices; i++)
				{
					Assets::Mesh::Vertex Vert;
					Vert.Position = Math::Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

					if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
						Vert.UV = Math::Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
					else
						Vert.UV = Math::Vector2(0.0f, 0.0f);

					// normals
					if (mesh->mNormals != NULL)
						Vert.Normal = Math::Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
					else
						Vert.Normal = Math::Vector3(0.0f, 0.0f, 0.0f);

					//tangents
					if (mesh->mTangents != NULL)
						Vert.Tangents = Math::Vector3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
					else
						Vert.Tangents = Math::Vector3(0.0f, 0.0f, 0.0f);

					//bitangents
					if (mesh->mBitangents != NULL)
						Vert.Bitangents = Math::Vector3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
					else
						Vert.Bitangents = Math::Vector3(0.0f, 0.0f, 0.0f);

					result->Vertices.push_back(Vert);
				}

				result->indices.reserve(mesh->mNumVertices);
				for (unsigned int i = 0; i < mesh->mNumFaces; i++)
				{
					aiFace face = mesh->mFaces[i];
					for (unsigned int j = 0; j < face.mNumIndices; j++)
					{
						result->indices.push_back(face.mIndices[j]);
					}
				}


				// process material		
				result->TexSetIndex = ProcessMaterial(mesh, scene);

				if (mModel.pAnimations && scene->mAnimations != nullptr)
				{
					ExtractBoneWeightForVertices(result, mesh, scene);
				}
			}

			Assets::TextureUsageType GetTextureType(aiTextureType type)
			{
				switch (type)
				{
				case aiTextureType_DIFFUSE:
					return Assets::TextureUsageType::Diffuse;
				case aiTextureType_SPECULAR:
					return Assets::TextureUsageType::Specular;
				case aiTextureType_HEIGHT:
					return Assets::TextureUsageType::Normal;
				}

				return Assets::TextureUsageType::Unknown;
			}
			Assets::MaterialTextureSet AssimpLoader::ProcessMaterialTexture(aiMaterial* mat, int arttype)
			{
				Assets::MaterialTextureSet textures;
				aiTextureType type = aiTextureType(arttype);

				for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
				{
					aiString str;
					mat->GetTexture(type, i, &str);

					Assets::MaterialTexture texture;

					//Embedded Texture
					if (auto embeddedtex = scene->GetEmbeddedTexture(str.C_Str()))
					{
						auto textype = GetTextureType(type);

						if (embeddedtex->mHeight != 0)
						{
							Assets::TextureDesc data;
							data.mWidth = embeddedtex->mWidth;
							data.mHeight = embeddedtex->mHeight;
							data.mData = (Byte*)embeddedtex->pcData;
							data.mPath = embeddedtex->mFilename.C_Str();
							texture.pTexture = Assets::Importer::GetInstance().ImportTexture(data);
							texture.mUsageType = textype;
						}
						else
						{
							TextureImportingDesc imagedesc;
							imagedesc.mLoadFromMemory = true;
							imagedesc.mMemData = (Byte*)embeddedtex->pcData;
							imagedesc.mMemSize = embeddedtex->mWidth;
							texture.pTexture = Assets::Importer::GetInstance().ImportTexture(embeddedtex->mFilename.C_Str(),imagedesc);
							texture.mUsageType = textype;
						}

					}
					else
					{
						//Load Texture normally
						std::string filename = str.C_Str();
						filename = mDirectory + '/' + filename;
						auto textype = GetTextureType(type);
						TextureImportingDesc teximportdesc;
						teximportdesc.mCommonOptions.mExportPath = exportPath + "Textures/";
						//teximportdesc.mCommonOptions.mAssetName = 
						texture.pTexture = Assets::Importer::GetInstance().ImportTexture(filename, teximportdesc);
						texture.mUsageType = textype;
					}

					textures.mData.push_back(texture);

				}
				return textures;
			}

			void AssimpLoader::LoadAnimations()
			{
				mModel.pAnimations->mClips.reserve(scene->mNumAnimations);
				for (Uint32 i = 0; i < scene->mNumAnimations; i++)
				{
					mModel.pAnimations->mClips.push_back(Animation::AnimationClip());
					auto clipptr = &mModel.pAnimations->mClips.at(0);
					auto animation = scene->mAnimations[i];

					clipptr->m_Duration = animation->mDuration;
					clipptr->m_TicksPerSecond = animation->mTicksPerSecond;

					aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
					globalTransformation = globalTransformation.Inverse();
					ReadHeirarchyData(&clipptr->mRootNode, scene->mRootNode);
					ReadMissingBones(animation, clipptr);
					return;
				}
			}

			void AssimpLoader::ExtractBoneWeightForVertices(Assets::Mesh::SubMesh::SubMeshData* meshdata, aiMesh* mesh, const aiScene* scene)
			{
				auto& boneInfoMap = mModel.pMesh->mBoneInfoMap;
				int& boneCount = mModel.pMesh->mBoneCounter;

				for (Uint32 boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
				{
					int boneID = -1;
					std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
					Uint32 bonehash = Utilities::Hash(boneName);

					if (boneInfoMap.find(bonehash) == boneInfoMap.end())
					{
						Animation::BoneInfo newBoneInfo;
						newBoneInfo.id = boneCount;
						newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
						boneInfoMap[bonehash] = newBoneInfo;
						boneID = boneCount;
						boneCount++;
					}
					else
					{
						boneID = boneInfoMap[bonehash].id;
					}
					assert(boneID != -1);
					auto weights = mesh->mBones[boneIndex]->mWeights;
					int numWeights = mesh->mBones[boneIndex]->mNumWeights;

					for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
					{
						int vertexId = weights[weightIndex].mVertexId;
						float weight = weights[weightIndex].mWeight;
						assert(vertexId <= meshdata->Vertices.size());
						SetVertexBoneData(meshdata->Vertices[vertexId], boneID, weight);
					}
				}
			}

			void AssimpLoader::ReadMissingBones(const aiAnimation* animation, Animation::AnimationClip* clip)
			{
				int size = animation->mNumChannels;

				auto& boneInfoMap = mModel.pMesh->mBoneInfoMap;//getting m_BoneInfoMap from Model class
				int& boneCount = mModel.pMesh->mBoneCounter; //getting the m_BoneCounter from Model class

				//reading channels(bones engaged in an animation and their keyframes)
				clip->mBones.reserve(size);
				for (int i = 0; i < size; i++)
				{
					auto channel = animation->mChannels[i];
					std::string boneName = channel->mNodeName.data;
					Uint32 bonehash = Utilities::Hash(boneName);

					if (boneInfoMap.find(bonehash) == boneInfoMap.end())
					{
						boneInfoMap[bonehash].id = boneCount;
						boneCount++;
					}
					Animation::Bone bone(channel->mNodeName.data, boneInfoMap[bonehash].id);
					InitBoneData(channel, bone.mData);
					clip->mBones.push_back(bone);
				}

				clip->mBoneInfoMap = boneInfoMap;
			}

			void AssimpLoader::ReadHeirarchyData(Animation::ClipNodeData* dest, const aiNode* src)
			{
				assert(src);

				dest->mName = src->mName.data;
				dest->mHashedName = Utilities::Hash(dest->mName);
				dest->transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
				dest->childrenCount = src->mNumChildren;

				dest->children.reserve(src->mNumChildren);
				for (Uint32 i = 0; i < src->mNumChildren; i++)
				{
					//dest->mRootNode.children.push_back(Animation::ClipNodeData());
					//dest->mRootNode.children.back();
					dest->children.push_back(Animation::ClipNodeData());
					ReadHeirarchyData(&dest->children.back(), src->mChildren[i]);
				}
			}

			void AssimpLoader::InitBoneData(const aiNodeAnim* channel, Animation::BoneData& result)
			{
				result.m_NumPositions = channel->mNumPositionKeys;

				for (int positionIndex = 0; positionIndex < result.m_NumPositions; ++positionIndex)
				{
					aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
					Float64 timeStamp = channel->mPositionKeys[positionIndex].mTime;
					Animation::KeyPosition data;
					data.position = AssimpGLMHelpers::GetGLMVec(aiPosition);
					data.timeStamp = timeStamp;
					result.m_Positions.push_back(data);
				}

				result.m_NumRotations = channel->mNumRotationKeys;
				for (int rotationIndex = 0; rotationIndex < result.m_NumRotations; ++rotationIndex)
				{
					aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
					Float64 timeStamp = channel->mRotationKeys[rotationIndex].mTime;
					Animation::KeyRotation data;
					data.orientation = AssimpGLMHelpers::GetGLMQuat(aiOrientation);
					data.timeStamp = timeStamp;
					result.m_Rotations.push_back(data);
				}

				result.m_NumScalings = channel->mNumScalingKeys;
				for (int keyIndex = 0; keyIndex < result.m_NumScalings; ++keyIndex)
				{
					aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
					Float64 timeStamp = channel->mScalingKeys[keyIndex].mTime;
					Animation::KeyScale data;
					data.scale = AssimpGLMHelpers::GetGLMVec(scale);
					data.timeStamp = timeStamp;
					result.m_Scales.push_back(data);
				}
			}


			/*	bool AssimpLoadMesh(const MeshImporterDesc& desc, Assets::Mesh* mesh, Assets::MaterialCreationInfo* material, Assets::Animations* anim)
				{
					AssimpImporter importer;
					return importer.Load(desc,mesh,material,anim);
				}*/
		}
	}
}