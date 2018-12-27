#pragma once
#include <Engine/Graphics/Color.h>
#include <Engine/Assets/Texture.h>
#include <Engine/Assets/Image.h>
#include <Diligent/Graphics/GraphicsEngine/interface/TextureView.h>
#include <Base\Utilities\Delegate.h>
#include <tuple>
#include <unordered_map>
#include <array>
#include <string>

namespace NuclearEngine {
	namespace Assets
	{
		class Mesh;
		class Material;
	}
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
			std::string mName;

			Diligent::USAGE mUsage;

			Diligent::BIND_FLAGS mBindFlags;

			Uint32 mMipLevels;

			Diligent::CPU_ACCESS_FLAGS mCPUAccessFlags;

			bool mIsSRGB;

			bool mGenerateMips;

			bool mFlipY_Axis;

			Diligent::TEXTURE_FORMAT mFormat;

			TextureLoadingDesc() :
				mName(""),
				mUsage(Diligent::USAGE_STATIC),
				mBindFlags(Diligent::BIND_SHADER_RESOURCE),
				mMipLevels(0),
				mCPUAccessFlags(Diligent::CPU_ACCESS_NONE),
				mIsSRGB(false),
				mGenerateMips(true),
				mFlipY_Axis(true),
				mFormat(Diligent::TEX_FORMAT_UNKNOWN)
			{}
		};
		struct AssetManagerDesc
		{
			std::string mName;
			ASSET_MANAGER_FLUSH_FLAGS mFlushFlagOnShutdown;
			bool mSaveTextureNames = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real texture name or not
			bool mSaveMeshNames = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real mesh name or not
			bool mSaveMaterialsNames = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real mesh name or not
		};

		typedef Utilities::Delegate<std::tuple<Assets::Mesh, Assets::Material>(const std::string& Path, const Managers::MeshLoadingDesc& desc)> MeshImport;
		typedef Utilities::Delegate<Assets::Image&(const std::string& Path, const TextureLoadingDesc & Desc)> TextureImport;
		
		class NEAPI AssetManager {
		public:
			AssetManager(AssetManagerDesc desc = AssetManagerDesc());
			~AssetManager();

			std::unordered_map<Uint32, Assets::Texture> mImportedTextures; //All loaded textures with their hashed names with crc32c (always saved)
			std::unordered_map<Uint32, std::string> mHashedTexturesNames; //Real pre-hashed texture names with paths (conditionally saved see SaveTextureNames)
			bool mSaveTextureNames = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real texture name or not

			std::unordered_map<Uint32, Assets::Mesh> mImportedMeshes; //All imported meshes with their hashed names with crc32c (always saved)
			std::unordered_map<Uint32, Assets::Material> mImportedMaterials; //All imported materials with their hashed names with crc32c (always saved)
			std::unordered_map<Uint32, std::string> mHashedMeshesMaterialsPaths; //Real pre-hashed meshes names with paths (conditionally saved see mSaveMeshNames) 
			bool mSaveMeshMaterialPaths = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real mesh name or not

			TextureImport mTextureImporter;
			MeshImport mMeshImporter;

			//Note: Automatically called on Destruction
			void FlushContainers(ASSET_MANAGER_FLUSH_FLAGS = ASSET_MANAGER_FLUSH_ALL);

			std::tuple<Assets::Mesh, Assets::Material> Import(const std::string& Path, const MeshLoadingDesc& desc, Assets::Material& material);
			Assets::Texture& Import(const std::string& Path, const TextureLoadingDesc& Desc = TextureLoadingDesc());
			Assets::Texture& Import(const std::string& Path, const Assets::TextureUsageType& type, const TextureLoadingDesc& Desc = TextureLoadingDesc());
			std::tuple<Assets::Mesh, Assets::Material> Import(const std::string& Path, const Managers::MeshLoadingDesc& desc);

			bool DoesTextureExist(Uint32 hashedname, Assets::Texture* texture);

			//Order:  [+X (right)] [-X (left)] [+Y (top)] [-Y (bottom)] [+Z (front)] [-Z (back)]			
			std::array<Diligent::ITexture*, 6> LoadTextureCubeFromFile(const std::array<std::string, 6 >& Paths, const TextureLoadingDesc& Desc);
		private:
			AssetManagerDesc mDesc;
			Diligent::ITexture* TextureCube_Load(const std::string& Path, const TextureLoadingDesc& Desc);
		};
	}
}