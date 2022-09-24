#pragma once
#include <Engine\Assets\Assets.h>
#include <Core/Path.h>
#include <unordered_map>

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

			void Release()
			{
				mData.clear();
				mHashedPaths.clear();
			}
		};

		struct NEAPI AssetLibrary
		{
			//Special type of asset??
			ImportedAssetsMap<Assets::Scene> mImportedScenes;

			ImportedAssetsMap<Assets::Image> mImportedImages;
			ImportedAssetsMap<Assets::Mesh> mImportedMeshes;
			ImportedAssetsMap<Assets::Material> mImportedMaterials;
			ImportedAssetsMap<Assets::Animations> mImportedAnimations;
			ImportedAssetsMap<Assets::AudioClip> mImportedAudioClips;
			ImportedAssetsMap<Assets::Font> mImportedFonts;
		};
	}
}