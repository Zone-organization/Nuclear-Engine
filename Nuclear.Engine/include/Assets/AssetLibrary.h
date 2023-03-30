#pragma once
#include <Assets/Mesh.h>
#include <Assets/DefaultMeshes.h>
#include <Assets/Material.h>
#include <Assets/MaterialTypes.h>
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
			std::unordered_map<Core::UUID, T> mData;

			T* GetAsset(const Core::UUID& uuid)
			{
				auto it = mData.find(uuid);
				if (it != mData.end())
				{
					return &it->second;
				}
				return nullptr;
			}


			T* GetOrAddAsset(const Core::UUID& uuid)
			{
				auto it = mData.find(uuid);
				if (it != mData.end())
				{
					return &it->second;
				}
				return &AddAsset(uuid);
			}

			T& AddAsset(const Core::UUID& uuid)
			{
				T& result = mData[uuid];
				result.SetUUID(uuid);
				return result;
			}

			T& AddAsset()
			{
				auto uuid = Core::UUID::CreateNewUUID();
				T& result = mData[uuid];
				result.SetUUID(uuid);
				return result;
			}

			void Release()
			{
				mData.clear();
			}			
		};

		class NEAPI AssetLibrary
		{
		public:
			AssetLibrary(AssetLibrary const&) = delete;
			void operator=(AssetLibrary const&) = delete;

			void Initialize(const std::string& path);

			const std::string& GetPath() const;

			//Special type of asset??
			ImportedAssetsMap<Assets::Scene> mImportedScenes;

			ImportedAssetsMap<Assets::Texture> mImportedTextures;
			ImportedAssetsMap<Assets::Mesh> mImportedMeshes;
			ImportedAssetsMap<Assets::Material> mImportedMaterials;
			ImportedAssetsMap<Assets::Animations> mImportedAnimations;
			ImportedAssetsMap<Assets::AudioClip> mImportedAudioClips;
			ImportedAssetsMap<Assets::Font> mImportedFonts;
			ImportedAssetsMap<Assets::Shader> mImportedShaders;
			ImportedAssetsMap<Assets::Script> mImportedScripts;

			static AssetLibrary& GetInstance();

			void Clear();

			template<typename S>
			void serialize(S& s)
			{
				s.object(mImportedTextures.mData);
			}

		private:	
			std::string mPath;
			AssetLibrary();
		};
	}
}