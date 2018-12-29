#pragma once
#include <Engine/Graphics/Color.h>
#include <Engine/Importers/Importers.h>
#include <unordered_map>
#include <array>

namespace NuclearEngine {
	namespace Managers
	{
		enum ASSET_MANAGER_FLUSH_FLAGS
		{
			ASSET_MANAGER_FLUSH_NONE = 0x00,
			ASSET_MANAGER_FLUSH_MESHES = 0x01,
			ASSET_MANAGER_FLUSH_MESHES_NAMES = 0x02,
			ASSET_MANAGER_FLUSH_TEXTURES = 0x03,
			ASSET_MANAGER_FLUSH_TEXTURES_NAMES = 0x04,
			ASSET_MANAGER_FLUSH_MATERIALS = 0x05,
			ASSET_MANAGER_FLUSH_MATERIALS_NAMES = 0x06,
			ASSET_MANAGER_FLUSH_ALL = 0x07,
		};
		DEFINE_FLAG_ENUM_OPERATORS(ASSET_MANAGER_FLUSH_FLAGS)


		struct AssetManagerDesc
		{
			std::string mPath;
			ASSET_MANAGER_FLUSH_FLAGS mFlushFlagOnShutdown;
			bool mSaveTexturePaths = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real texture name or not
			bool mSaveMeshPaths = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real mesh name or not
			bool mSaveMaterialsPaths = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real mesh name or not
		};

		class NEAPI AssetManager {
		public:
			AssetManager(AssetManagerDesc desc = AssetManagerDesc());
			~AssetManager();

			std::unordered_map<Uint32, Assets::Texture> mImportedTextures; //All loaded textures with their hashed names with crc32c (always saved)
			std::unordered_map<Uint32, std::string> mHashedTexturesPaths; //Real pre-hashed texture paths (conditionally saved see SaveTexturePaths)
			bool mSaveTexturesPaths = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real texture name or not

			std::unordered_map<Uint32, Assets::Mesh> mImportedMeshes; //All imported meshes with their hashed names with crc32c (always saved)
			std::unordered_map<Uint32, std::string> mHashedMeshesPaths; //Real pre-hashed meshes paths (conditionally saved see mSaveMeshPaths)
			bool mSaveMeshesPaths = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real meshes Paths or not

			std::unordered_map<Uint32, Assets::Material> mImportedMaterials; //All imported materials with their hashed names with crc32c (always saved)
			std::unordered_map<Uint32, std::string> mHashedMaterialsPaths; //Real pre-hashed materials paths (conditionally saved see mSaveMaterialPaths)
			bool mSaveMaterialsPaths = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real materials Paths or not

			bool mMultithreadMeshTextureLoading = true;

			Importers::TextureImporterDelegate mTextureImporter;
			Importers::MeshImporterDelegate mMeshImporter;

			//Note: Automatically called on Destruction
			void FlushContainers(ASSET_MANAGER_FLUSH_FLAGS = ASSET_MANAGER_FLUSH_ALL);

			Assets::Texture& Import(const std::string& Path, const Importers::TextureLoadingDesc& Desc = Importers::TextureLoadingDesc());
			Assets::Texture& Import(const std::string& Path, const Assets::TextureUsageType& type, const Importers::TextureLoadingDesc& Desc = Importers::TextureLoadingDesc());
			Assets::Texture& Import(const Assets::Image& Image, const Importers::TextureLoadingDesc& Desc = Importers::TextureLoadingDesc());

			std::tuple<Assets::Mesh, Assets::Material> Import(const std::string& Path, const Importers::MeshLoadingDesc& desc);
			
			bool DoesTextureExist(Uint32 hashedname, Assets::Texture* texture);

			//Order:  [+X (right)] [-X (left)] [+Y (top)] [-Y (bottom)] [+Z (front)] [-Z (back)]			
			std::array<ITexture*, 6> LoadTextureCubeFromFile(const std::array<std::string, 6 >& Paths, const Importers::TextureLoadingDesc& Desc);
		private:
			AssetManagerDesc mDesc;
			ITexture* TextureCube_Load(const std::string& Path, const Importers::TextureLoadingDesc& Desc);
		};
	}
}