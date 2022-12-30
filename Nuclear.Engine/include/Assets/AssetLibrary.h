#pragma once
#include <Assets/Model.h>
#include <Assets/Mesh.h>
#include <Assets/DefaultMeshes.h>
#include <Assets/Material.h>
#include <Assets/MaterialTypes.h>
#include <Assets/ImageData.h>
#include <Assets/Animations.h>
#include <Assets/AudioClip.h>
#include <Assets/Script.h>
#include <Assets/Scene.h>
#include <Assets/Shader.h>
#include <Assets/Font.h>
#include <Core/Path.h>
#include <Serialization/AssetsSerialization.h>

namespace Nuclear
{
	namespace Assets
	{
		template <class T>
		struct ImportedAssetsMap
		{
			std::unordered_map<Uint32, T> mData; //All loaded asset T with their hashed names with crc32c (always saved)

			std::unordered_map<Uint32, Core::Path> mHashedPaths; //Real pre-hashed paths (conditionally saved see mSavePaths)
			bool mSavePaths = DEBUG_TRUE_BOOL;                   //tells the asset manager whether to store the real path or not in 'mHashedPaths'

			void SavePath(Uint32 hashed, const Core::Path& path)
			{
				if (mSavePaths)
				{
					mHashedPaths[hashed] = path;
				}
			}

			T* GetAsset(Uint32 hashedpath)
			{
				auto it = mData.find(hashedpath);
				if (it != mData.end())
				{
					return &it->second;
				}
				return nullptr;
			}


			T* GetOrAddAsset(Uint32 hashedpath)
			{
				auto it = mData.find(hashedpath);
				if (it != mData.end())
				{
					return &it->second;
				}
				return &AddAsset(hashedpath);
			}

			T& AddAsset(Uint32 hashed)
			{
				T& result = mData[hashed];
				result.SetUUID(Utilities::UUID::CreateNewUUID());
				return result;
			}

			T& AddAsset(const Core::Path& path, Uint32 hashed)
			{
				if (mSavePaths)
					mHashedPaths[hashed] = path;

				return AddAsset(hashed);
			}

			void Release()
			{
				mData.clear();
				mHashedPaths.clear();
			}			
		};

		class NEAPI AssetLibrary
		{
		public:
			AssetLibrary(AssetLibrary const&) = delete;
			void operator=(AssetLibrary const&) = delete;

			std::string mName = "DefaultLibrary";
			std::string mPath = "";

			//Special type of asset??
			ImportedAssetsMap<Assets::Scene> mImportedScenes;

			ImportedAssetsMap<Assets::Image> mImportedImages;
			ImportedAssetsMap<Assets::Model> mImportedModels;
			ImportedAssetsMap<Assets::Mesh> mImportedMeshes;
			ImportedAssetsMap<Assets::Material> mImportedMaterials;
			ImportedAssetsMap<Assets::MaterialData> mImportedMaterialDatas;
			ImportedAssetsMap<Assets::Animations> mImportedAnimations;
			ImportedAssetsMap<Assets::AudioClip> mImportedAudioClips;
			ImportedAssetsMap<Assets::Font> mImportedFonts;
			ImportedAssetsMap<Assets::Shader> mImportedShaders;
			ImportedAssetsMap<Assets::Script> mImportedScripts;

			static AssetLibrary& GetInstance();


			template<typename S>
			void serialize(S& s)
			{
				s.object(mImportedImages.mData);
			}

		private:		
			AssetLibrary();
		};
	}
}