#pragma once
#include <Engine/Graphics/Color.h>
#include <Engine/Assets/Texture.h>
#include <Engine/Assets/Image.h>
#include <Engine/Assets/Mesh.h>
#include <Engine/Assets/Material.h>
#include <Base\Utilities\Delegate.h>
#include <tuple>
#include <unordered_map>
#include <array>
#include <string>

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

		struct MeshLoadingDesc
		{
			bool UseTexCoords = true;
			bool UseNormals = true;
			bool UseTangents = false;
			bool LoadDiffuseTextures = true;
			bool LoadSpecularTextures = false;
			bool LoadNormalTextures = false;
		};
		struct TextureLoadingDesc
		{
			std::string mPath;

			USAGE mUsage;

			BIND_FLAGS mBindFlags;

			Uint32 mMipLevels;

			CPU_ACCESS_FLAGS mCPUAccessFlags;

			bool mIsSRGB;

			bool mGenerateMips;

			bool mFlipY_Axis;

			TEXTURE_FORMAT mFormat;

			TextureLoadingDesc() :
				mPath(""),
				mUsage(USAGE_STATIC),
				mBindFlags(BIND_SHADER_RESOURCE),
				mMipLevels(0),
				mCPUAccessFlags(CPU_ACCESS_NONE),
				mIsSRGB(false),
				mGenerateMips(true),
				mFlipY_Axis(true),
				mFormat(TEX_FORMAT_UNKNOWN)
			{}
		};
		struct AssetManagerDesc
		{
			std::string mPath;
			ASSET_MANAGER_FLUSH_FLAGS mFlushFlagOnShutdown;
			bool mSaveTexturePaths = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real texture name or not
			bool mSaveMeshPaths = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real mesh name or not
			bool mSaveMaterialsPaths = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real mesh name or not
		};

		typedef Utilities::Delegate<std::tuple<Assets::Mesh, Assets::Material>(const std::string& Path, const Managers::MeshLoadingDesc& desc)> MeshImport;
		typedef Utilities::Delegate<Assets::Image&(const std::string& Path, const TextureLoadingDesc & Desc)> TextureImport;
		
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


			TextureImport mTextureImporter;
			MeshImport mMeshImporter;

			//Note: Automatically called on Destruction
			void FlushContainers(ASSET_MANAGER_FLUSH_FLAGS = ASSET_MANAGER_FLUSH_ALL);

			Assets::Texture& Import(const std::string& Path, const TextureLoadingDesc& Desc = TextureLoadingDesc());
			Assets::Texture& Import(const std::string& Path, const Assets::TextureUsageType& type, const TextureLoadingDesc& Desc = TextureLoadingDesc());
			Assets::Texture& Import(const Assets::Image& Image, const TextureLoadingDesc& Desc = TextureLoadingDesc());

			std::tuple<Assets::Mesh, Assets::Material> Import(const std::string& Path, const Managers::MeshLoadingDesc& desc);



			bool DoesTextureExist(Uint32 hashedname, Assets::Texture* texture);

			//Order:  [+X (right)] [-X (left)] [+Y (top)] [-Y (bottom)] [+Z (front)] [-Z (back)]			
			std::array<ITexture*, 6> LoadTextureCubeFromFile(const std::array<std::string, 6 >& Paths, const TextureLoadingDesc& Desc);
		private:
			AssetManagerDesc mDesc;
			ITexture* TextureCube_Load(const std::string& Path, const TextureLoadingDesc& Desc);
		};
	}
}