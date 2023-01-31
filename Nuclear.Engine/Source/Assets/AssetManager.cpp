#include <Assets\AssetManager.h>
#include <Utilities/Logger.h>

#include <Platform\FileSystem.h>

#include <Serialization/SerializationEngine.h>
#include <Assets/AssetLibrary.h>

#include <filesystem>
#include <Assets\Loader.h>

#include <ThirdParty/magic_enum.hpp>
#include <Assets/Importers/AssimpManager.h>

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
			
		}

		void AssetManager::Initialize(AssetManagerDesc desc)
		{
			mDesc = desc;
		}
		
		void AssetManager::LoadFolder(const Core::Path& Path)
		{
			namespace fs = std::filesystem;

			for (const auto& entry : fs::directory_iterator(Path.GetRealPath()))
			{
				if (!entry.is_directory())
				{
					if (entry.path().extension() == ".NEMeta")
					{
						AssetMetadata meta;
						if (!Serialization::SerializationEngine::GetInstance().Deserialize(meta, entry.path().string()))
						{
							NUCLEAR_ERROR("[AssetManager] LoadFolder {0} Asset metadata deserialization failed! : {1}", Path.GetInputPath(), entry.path().string());
						}

						Load(entry.path().string(), meta, false);
					}
				}
				else
				{
					//load subfolders
					LoadFolder(entry.path().string());
				}
			}
		}

		IAsset* AssetManager::Load(const Core::Path& Path)
		{
			std::string path = Path.GetRealPath();
			if (Path.GetExtension() != ".NEMeta")
			{
				path = path + ".NEMeta";
			}
			AssetMetadata meta;
			if (!Serialization::SerializationEngine::GetInstance().Deserialize(meta, path))
			{
				NUCLEAR_ERROR("[AssetManager] Loading Asset with no metadata! : '{0}'", Path.GetInputPath());
				return nullptr;
			}

			return Load(Path, meta, true);
		}

		IAsset* AssetManager::Load(const Core::Path& Path, const AssetMetadata& meta, bool auto_load_assets_dependencies)
		{
			if (meta.mType == AssetType::Texture)
			{
				return Loader::GetInstance().LoadTexture(Path, meta);
			}
			else if (meta.mType == AssetType::Material)
			{
				return Loader::GetInstance().LoadMaterial(Path, meta);
			}
			else if (meta.mType == AssetType::Shader)
			{
				return Loader::GetInstance().LoadShader(Path, meta);
			}
			else if (meta.mType == AssetType::Mesh)
			{
				return Loader::GetInstance().LoadMesh(Path, auto_load_assets_dependencies,meta);
			}
			return nullptr;
		}


		IAsset* AssetManager::Import(const Core::Path& Path, AssetType type)
		{
			AssetType Type = type;
			if (Type == AssetType::Unknown)
			{
				Type = Importer::GetInstance().GetAssetType(Path.GetRealPath());
			}			
			else if (Type == AssetType::Texture)
			{
				return Importer::GetInstance().ImportTexture(Path);
			}
			else if (Type == AssetType::Mesh)
			{
				return Importer::GetInstance().ImportMesh(Path);
			}
			else if (Type == AssetType::Material)
			{
				return Importer::GetInstance().ImportMaterial(Path);
			}
			else if (Type == AssetType::Scene)
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
				else
				{
					//load subfolders
					ImportFolder(dir_entry.path().string());
				}
			}
		}
				
		AssetMetadata AssetManager::CreateMetadata(IAsset* asset)
		{
			AssetMetadata result;

			result.mName = asset->GetName();
			result.mUUID = asset->GetUUID();
			result.mType = asset->GetType();

			return result;
		}

		bool AssetManager::Export(const Serialization::BinaryBuffer& asset, const Core::Path& Path)
		{
			Platform::FileSystem::GetInstance().SaveBinaryBuffer(asset, Path);

			return false;
		}

		std::string GetExtensionByType(AssetType Type)
		{		
			if (Type == AssetType::Material)
			{
				return ".NEMaterial";
			}
			else if (Type == AssetType::Scene)
			{
				return ".NEScene";
			}
			else if (Type == AssetType::Shader)
			{
				return ".NEShader";
			}
			return "";
		}

		bool AssetManager::Export(IAsset* asset, bool exportmetadata, const Core::Path& Path)
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

			if (!asset->GetUUID().isValid())
			{
				NUCLEAR_WARN("[AssetManager] Exporting an asset '{0}' with invalid UUID -> assigning a new one automatically...", Path.GetInputPath());
				asset->mUUID = Core::UUID::CreateNewUUID();
			}
			if (exportmetadata)
			{
				auto assetmetadata = CreateMetadata(asset);
				Serialization::SerializationEngine::GetInstance().Serialize(assetmetadata, exportpath.GetRealPath() + asset->GetName() + GetExtensionByType(asset->GetType()) + ".NEMeta");
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
			else if (asset->GetType() == AssetType::Shader)
			{
				auto shader = static_cast<Shader*>(asset);

				if (shader)
				{
					Serialization::BinaryBuffer buffer;
					zpp::bits::out out(buffer);
					out(*shader);

					Platform::FileSystem::GetInstance().SaveBinaryBuffer(buffer, exportpath.GetRealPath() + shader->GetName() + ".NEShader");
					NUCLEAR_INFO("[AssetManager] Exported Shader '{0}' To: '{1}'", shader->GetName(), exportpath.GetInputPath());
					return true;
				}
			}
			//WIP:
			//Convert all assets to unified types.
			return false;
		}
	
	}
}