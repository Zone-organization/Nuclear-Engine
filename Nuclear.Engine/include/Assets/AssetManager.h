#pragma once
#include <Assets/ImportingDescs.h>
#include <Assets\AssetMetadata.h>
#include <Assets\Importer.h>
#include <Threading\ThreadPool.h>
#include <Serialization/BinaryBuffer.h>
#include <FMOD/inc/fmod.hpp>
#include <array>

#define MAX_BONE_INFLUENCE 4
#define ASSET_METHODS_SPECIALIZATION(ASSETTYPE)   \
template <> ASSETTYPE* Import<ASSETTYPE>(const Core::Path& Path) { \
	return Importer::GetInstance().Import##ASSETTYPE(Path); } \
template <> ASSETTYPE* Import<ASSETTYPE>(const Core::Path& Path, const ASSETTYPE##ImportingDesc & desc) { \
return Importer::GetInstance().Import##ASSETTYPE(Path, desc); } \

namespace Nuclear 
{
	namespace Assets
	{
		struct AssetManagerDesc
		{
			bool mSaveTexturePaths = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real texture name or not
			bool mSaveMeshPaths = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real mesh name or not
			bool mSaveMaterialsPaths = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real material name or not
		};

		//Importing vs Loading
		//Importing: using asset for the first time ( no metadata)
		//Loading: loading a previously imported "exported" asset with metadata
		class NEAPI AssetManager
		{
		public:
			AssetManager(AssetManager const&) = delete;
			void operator=(AssetManager const&) = delete;

			static AssetManager& GetInstance();

			//Note: Automatically called on Destruction
			void FlushContainers();
			void Initialize(AssetManagerDesc desc = AssetManagerDesc());

			//Generic

			template<class T>
			T* Load(const Core::Path& Path)
			{
				static_assert(std::is_base_of<IAsset, T>::value, "Import<T> class must derive from IAsset!");

				return static_cast<T*>(Load(Path));
			}

			IAsset* Load(const Core::Path& Path);

			IAsset* Load(const Core::Path& Path, const AssetMetadata& meta);


			template<class T>
			T* Import(const Core::Path& Path)
			{
				static_assert(std::is_base_of<IAsset, T>::value, "Import<T> class must derive from IAsset!");

				return static_cast<T*>(Import(Path));
			}

			template<class T, class D>
			T* Import(const Core::Path& Path, const D& desc)
			{
				static_assert(std::is_base_of<IAsset, T>::value, "Import<T> class must derive from IAsset!");

				return static_cast<T*>(Import(Path));
			}

			IAsset* Import(const Core::Path& Path, AssetType type = AssetType::Unknown);

			void ImportFolder(const Core::Path& Path);

			AssetMetadata CreateMetadata(IAsset* asset);
			bool Export(const Serialization::BinaryBuffer& buffer, const Core::Path& Path);
			bool Export(IAsset* asset, bool exportmetadata = true, const Core::Path& Path = "");
		protected:
			AssetManagerDesc mDesc;

		private:
			AssetManager();

		public:
			//Importing methods specializations

				ASSET_METHODS_SPECIALIZATION(Texture)
				ASSET_METHODS_SPECIALIZATION(Script)
				ASSET_METHODS_SPECIALIZATION(Model)
				ASSET_METHODS_SPECIALIZATION(Shader)
				ASSET_METHODS_SPECIALIZATION(Font)
				ASSET_METHODS_SPECIALIZATION(AudioClip)
				ASSET_METHODS_SPECIALIZATION(Scene)

		};
	}
}