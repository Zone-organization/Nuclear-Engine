#define EXPOSE_ASSIMP_IMPORTER
#include <Engine\Importers\AssimpImporter.h>
#include <Engine\Managers\AssetManager.h>
#include "Engine\Graphics\GraphicsEngine.h"
#include <Engine\Assets\Material.h>
#include <Core\Utilities\Hash.h>
#include "AssimpGLMHelpers.h"
#include <Core\Logger.h>

namespace Nuclear {
	namespace Importers {

		inline void SetVertexBoneData(Assets::Mesh::Vertex& vertex, int boneID, float weight)
		{
			vertex.Weights = Math::Vector4(weight);
			vertex.BoneIDs = Math::Vector4i(boneID);
		}

		bool AssimpImporter::Load(const MeshImporterDesc& desc, Assets::Mesh* mesh, Assets::Material* material, Assets::Animations* anim)
		{
			std::string path = desc.mPath;
			mLoadingDesc = desc.mMeshDesc;
			mManager = desc.mManager;
			mMesh = mesh;
			mMaterial = material;
			mAnimation = anim;
			Assimp::Importer importer;
			scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

			//Failed?
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				NUCLEAR_ERROR("[AssimpImporter] Assimp Failed to load mesh: '{0}' Info: '{1}'" , path, importer.GetErrorString());
				return false;
			}
			mDirectory = path.substr(0, path.find_last_of('/'));
			ProcessNode(scene->mRootNode, scene);

			if (mLoadingDesc.LoadAnimation && scene->mAnimations != nullptr)
			{
				LoadAnimations();
				mAnimation->isValid = true;
			}

			auto hashedname = Utilities::Hash(path);

			return true;
		}

		void AssimpImporter::ProcessNode(aiNode * node, const aiScene * scene)
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
		Uint32 AssimpImporter::ProcessMaterial(aiMesh* mesh, const aiScene* scene)
		{
			if (mLoadingDesc.LoadMaterial)
			{
				Assets::TextureSet TexSet;

				// process materials
				aiMaterial* MeshMat = scene->mMaterials[mesh->mMaterialIndex];
				auto MeshName = std::string(MeshMat->GetName().C_Str());
				TexSet.mHashedName = Utilities::Hash(MeshName);


				for (Uint32 i = 0; i < mMaterial->mPixelShaderTextures.size(); i++)
				{
					if (TexSet.mHashedName == mMaterial->mPixelShaderTextures.at(i).mHashedName)
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

					this->mMaterial->mPixelShaderTextures.push_back(TexSet);

					return static_cast<Uint32>(this->mMaterial->mPixelShaderTextures.size() - 1);
				}
			}
			return 0;
		}

		void AssimpImporter::ProcessMesh(aiMesh * mesh, const aiScene * scene)
		{
			assert(mMesh);
			mMesh->mSubMeshes.push_back(Assets::Mesh::SubMesh::SubMeshData());

			Assets::Mesh::SubMesh::SubMeshData* result  = &mMesh->mSubMeshes.back().data;
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

			if (mLoadingDesc.LoadAnimation && scene->mAnimations != nullptr)
			{
				ExtractBoneWeightForVertices(result, mesh, scene);
			}

		}

		Graphics::TextureUsageType GetTextureType(aiTextureType type)
		{
			switch (type)
			{
			case aiTextureType_DIFFUSE:
				return Graphics::TextureUsageType::Diffuse;
			case aiTextureType_SPECULAR:
				return Graphics::TextureUsageType::Specular;
			case aiTextureType_HEIGHT:
				return Graphics::TextureUsageType::Normal;
			}

			return Graphics::TextureUsageType::Unknown;
		}
		Assets::TextureSet AssimpImporter::ProcessMaterialTexture(aiMaterial * mat,const aiTextureType& type)
		{
			Assets::TextureSet textures;
			for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString str;
				mat->GetTexture(type, i, &str);

				Importers::ImageLoadingDesc desc;
				Graphics::Texture texture;

				if (auto embeddedtex = scene->GetEmbeddedTexture(str.C_Str()))
				{
					//Embedded Texture

					desc.mPath = embeddedtex->mFilename.C_Str();

					if (embeddedtex->mHeight != 0)
					{
						Assets::ImageData data;
						data.mWidth = embeddedtex->mWidth;
						data.mHeight = embeddedtex->mHeight;
						data.mData = (Byte*)embeddedtex->pcData;

						/*if (Graphics::GraphicsEngine::isGammaCorrect())
							desc.mFormat = TEX_FORMAT_RGBA8_UNORM_SRGB;
						else 
							desc.mFormat = TEX_FORMAT_RGBA8_UNORM;
						*/
						texture = mManager->Import(data, desc);
						texture.SetUsageType(GetTextureType(type));
					}
					else
					{
						desc.mLoadFromMemory = true;
						desc.mMemData = (Byte*)embeddedtex->pcData;
						desc.mMemSize = embeddedtex->mWidth;
						texture = mManager->Import(desc.mPath, desc, GetTextureType(type));
					}

				}
				else
				{
					//Load Texture

					std::string filename = str.C_Str();
					filename = mDirectory + '/' + filename;
					/*if (Graphics::GraphicsEngine::isGammaCorrect())
					{
						desc.mFormat = TEX_FORMAT_RGBA8_UNORM_SRGB;
					}
					else {
						desc.mFormat = TEX_FORMAT_RGBA8_UNORM;
					}*/
					texture = mManager->Import(filename, desc, GetTextureType(type));
				}

				textures.mData.push_back({ 0, texture });

			}
			return textures;
		}

		void AssimpImporter::LoadAnimations()
		{
			mAnimation->mClips.reserve(scene->mNumAnimations);
			for (Uint32 i = 0; i < scene->mNumAnimations; i++)
			{			
				mAnimation->mClips.push_back(Animation::AnimationClip());
				auto clipptr = &mAnimation->mClips.at(0);
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

		void AssimpImporter::ExtractBoneWeightForVertices(Assets::Mesh::SubMesh::SubMeshData* meshdata, aiMesh* mesh, const aiScene* scene)
		{
			auto& boneInfoMap = mMesh->mBoneInfoMap;
			int& boneCount = mMesh->mBoneCounter;

			for (Uint32 boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
			{
				int boneID = -1;
				std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
				if (boneInfoMap.find(boneName) == boneInfoMap.end())
				{
					Animation::BoneInfo newBoneInfo;
					newBoneInfo.id = boneCount;
					newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
					boneInfoMap[boneName] = newBoneInfo;
					boneID = boneCount;
					boneCount++;
				}
				else
				{
					boneID = boneInfoMap[boneName].id;
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

		void AssimpImporter::ReadMissingBones(const aiAnimation* animation, Animation::AnimationClip* clip)
		{
			int size = animation->mNumChannels;

			auto& boneInfoMap = mMesh->mBoneInfoMap;//getting m_BoneInfoMap from Model class
			int& boneCount = mMesh->mBoneCounter; //getting the m_BoneCounter from Model class

												   //reading channels(bones engaged in an animation and their keyframes)
			clip->mBones.reserve(size);
			for (int i = 0; i < size; i++)
			{
				auto channel = animation->mChannels[i];
				std::string boneName = channel->mNodeName.data;

				if (boneInfoMap.find(boneName) == boneInfoMap.end())
				{
					boneInfoMap[boneName].id = boneCount;
					boneCount++;
				}
				Animation::Bone bone(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].id);
				InitBoneData(channel, bone.mData);
				clip->mBones.push_back(bone);
			}

			clip->mBoneInfoMap = boneInfoMap;
		}

		void AssimpImporter::ReadHeirarchyData(Animation::ClipNodeData* dest, const aiNode* src)
		{
			assert(src);

			dest->name = src->mName.data;
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

		void AssimpImporter::InitBoneData(const aiNodeAnim* channel, Animation::BoneData& result)
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


		bool AssimpLoadMesh(const MeshImporterDesc& desc, Assets::Mesh* mesh, Assets::Material* material, Assets::Animations* anim)
		{
			AssimpImporter importer;
			return importer.Load(desc,mesh,material,anim);
		}
	}
}