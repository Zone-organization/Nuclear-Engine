#include <Assets\AssetManager.h>
#include <Utilities/Logger.h>

#include <Platform\FileSystem.h>

#include <Serialization/SerializationEngine.h>
#include <Assets/AssetLibrary.h>

#include <filesystem>

#include <ThirdParty/magic_enum.hpp>
namespace Nuclear
{
	namespace Assets
	{
	//	AssetManager::AssetManager(AssetManagerDesc desc)
	//		: mDesc(desc), mThreadPool(4)
	//	{
			//mImageImporter = ImageImporterDelegate::create<&FreeImageLoad>();
	//		//mMeshImporter = MeshImporterDelegate::create<&AssimpLoadMesh>();
	//		FT_Handle = nullptr;
	//	}


		//AssetManager::~AssetManager()
		//{
		//	FlushContainers(mDesc.mFlushFlagOnShutdown);
		//	msdfgen::deinitializeFreetype(FT_Handle);
		//}
		AssetManager::AssetManager()
		{
			
		}
		AssetManager& AssetManager::GetInstance()
		{
			static AssetManager instance;

			return instance;
		}

		void AssetManager::FlushContainers()
		{
			/*for (auto& it : mLibrary.mImportedMaterials) {
				 Do stuff

				for (auto& it1 : it.second.mPixelShaderTextures) {
					for (auto& it2 : it1.mData) {
						it2.mTex.mTextureView.RawPtr()->Release

						it2.mTex.mTextureView.Release();
					}
				}
				it.second.mTextureView.Release();
			}*/
		//
			/*mLibrary.mImportedMeshes.Release();

			mLibrary.mImportedImages.Release();

			mLibrary.mImportedMaterials.Release();

			mLibrary.mImportedAnimations.Release();

			mLibrary.mImportedAudioClips.Release();*/
		}

		void AssetManager::Initialize(AssetManagerDesc desc)
		{
			mDesc = desc;
		}

		//IAsset* AssetManager::Load(const Core::Path& Path)
		//{
		//	std::string path(Path.GetPathNoExt() + ".NEMeta");
		//	AssetMetadata meta;
		//	if (!Serialization::SerializationEngine::GetInstance().Deserialize(meta, path))
		//	{

		//	}

		////	return Import(Path, meta);
		//}

		IAsset* AssetManager::Import(const Core::Path& Path, AssetType type)
		{
			AssetType Type = type;
			if (Type == AssetType::Unknown)
			{
				Type = Importer::GetInstance().GetAssetType(Path.GetRealPath());
			}
			
			if (Type == AssetType::Texture)
			{
				return Importer::GetInstance().ImportTexture(Path);
			}
			if (Type == AssetType::Mesh)
			{
				return Importer::GetInstance().ImportModel(Path);
			}
			if (Type == AssetType::Material)
			{
				return Importer::GetInstance().ImportMaterial(Path);
			}
			if (Type == AssetType::Scene)
			{
				//return Importer::GetInstance().ImportTexture(Path);
			}
			return nullptr;
		}

		void AssetManager::ImportFolder(const Core::Path& Path)
		{
			const std::filesystem::path sandbox{ Path.GetRealPath()};

			for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ sandbox })
			{
				if (!dir_entry.is_directory())
				{
					auto filepath = dir_entry.path().string();
					auto assettype = Importer::GetInstance().GetAssetType(filepath);
					Import(filepath, assettype);
				}

			}
		}

	/*	Graphics::Texture AssetManager::ImportTexture(const Core::Path& Path, Graphics::TextureUsageType texturetype)
		{
			TextureImportingDesc desc;
			desc.mType = texturetype;
			return ImportTexture(Path, desc);
		}

		Graphics::Texture AssetManager::ImportTexture(const Core::Path& Path, const TextureImportingDesc& Desc)
		{
			return Importer::GetInstance().ImportTexture(Path, Desc);
		}

		Graphics::Texture AssetManager::ImportTexture(const TextureDesc& Imagedata, const TextureImportingDesc& Desc)
		{
			return Importer::GetInstance().ImportTexture(Imagedata, Desc);
		}*/

		//IAsset* AssetManager::Load(const Core::Path& Path, const AssetMetadata& meta)
		//{
		//	if (meta.mType == AssetType::Image)
		//	{
		//		return Importer::GetInstance().ImportImage(Path, );
		//	}
		//	return nullptr;
		//}

		
		AssetMetadata AssetManager::CreateMetadata(IAsset* asset)
		{
			AssetMetadata result;

			result.mName = asset->GetName();
			result.mHashedName = Utilities::Hash(asset->GetName());
			result.mUUID = asset->GetUUID();
			result.mType = asset->GetType();

			return result;
		}

		bool AssetManager::Export(const Serialization::BinaryBuffer& asset, const Core::Path& Path)
		{
			Platform::FileSystem::GetInstance().SaveBinaryBuffer(asset, Path);

			return false;
		}

		bool AssetManager::Export(IAsset* asset, const Core::Path& Path)
		{
			Core::Path exportpath = Path;
			if (!Path.isValid())
			{
				std::string assettypestr(magic_enum::enum_name(asset->GetType()));

				exportpath = AssetLibrary::GetInstance().GetPath() + assettypestr + "s/";
			}


			if (!asset)
			{
				NUCLEAR_ERROR("[AssetManager] Exporting a null asset! To : '{0}'", Path.GetInputPath());
				return false;
			}


			if (asset->GetType() == AssetType::Scene)
			{
				auto scene = static_cast<Scene*>(asset);
				if (!scene->mBinaryBuffer.empty())
				{
					Platform::FileSystem::GetInstance().SaveBinaryBuffer(scene->mBinaryBuffer, exportpath);
					NUCLEAR_INFO("[AssetManager] Exported Scene '{0}' To: '{1}'", scene->GetName(), exportpath.GetInputPath());
					return true;
				}
			}
			else  if (asset->GetType() == AssetType::Material)
			{
				 auto material = static_cast<Material*>(asset);

				 if (material)
				 {
					 Serialization::BinaryBuffer buffer;
					 zpp::bits::out out(buffer);
					 out(*material);

					 Platform::FileSystem::GetInstance().SaveBinaryBuffer(buffer, exportpath.GetRealPath() + material->GetName() + ".NEMaterial");
					 NUCLEAR_INFO("[AssetManager] Exported Material '{0}' To: '{1}'", material->GetName(), exportpath.GetInputPath());
					 return true;
				 }
			}

			
			//WIP:
			//Convert all assets to unified types.
			return false;
		}
	}
}