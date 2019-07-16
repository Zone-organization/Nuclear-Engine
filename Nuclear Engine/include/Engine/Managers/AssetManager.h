#pragma once
#include <Engine/Graphics/Color.h>
#include <Engine/Importers/Common.h>
#include <Engine\Assets\AudioClip.h>
#include <FMOD/inc/fmod.hpp>
#include <Core/Path.h>
#include <unordered_map>
#include <map>
#include <array>

namespace NuclearEngine 
{
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


		enum AUDIO_IMPORT_MODE
		{
			AUDIO_IMPORT_MODE_DEFAULT = 0x00000000,
			AUDIO_IMPORT_MODE_LOOP_OFF = 0x00000001,
			AUDIO_IMPORT_MODE_LOOP_NORMAL = 0x00000002,
			AUDIO_IMPORT_MODE_LOOP_BIDI = 0x00000004,
			AUDIO_IMPORT_MODE_2D = 0x00000008,
			AUDIO_IMPORT_MODE_3D = 0x00000010,
			AUDIO_IMPORT_MODE_CREATESTREAM = 0x00000080,
			AUDIO_IMPORT_MODE_CREATESAMPLE = 0x00000100,
			AUDIO_IMPORT_MODE_CREATECOMPRESSEDSAMPLE = 0x00000200,
			AUDIO_IMPORT_MODE_OPENUSER = 0x00000400,
			AUDIO_IMPORT_MODE_OPENMEMORY = 0x00000800,
			AUDIO_IMPORT_MODE_OPENMEMORY_POINT = 0x10000000,
			AUDIO_IMPORT_MODE_OPENRAW = 0x00001000,
			AUDIO_IMPORT_MODE_OPENONLY = 0x00002000,
			AUDIO_IMPORT_MODE_ACCURATETIME = 0x00004000,
			AUDIO_IMPORT_MODE_MPEGSEARCH = 0x00008000,
			AUDIO_IMPORT_MODE_NONBLOCKING = 0x00010000,
			AUDIO_IMPORT_MODE_UNIQUE = 0x00020000,
			AUDIO_IMPORT_MODE_3D_HEADRELATIVE = 0x00040000,
			AUDIO_IMPORT_MODE_3D_WORLDRELATIVE = 0x00080000,
			AUDIO_IMPORT_MODE_3D_INVERSEROLLOFF = 0x00100000,
			AUDIO_IMPORT_MODE_3D_LINEARROLLOFF = 0x00200000,
			AUDIO_IMPORT_MODE_3D_LINEARSQUAREROLLOFF = 0x00400000,
			AUDIO_IMPORT_MODE_3D_INVERSETAPEREDROLLOFF = 0x00800000,
			AUDIO_IMPORT_MODE_3D_CUSTOMROLLOFF = 0x04000000,
			AUDIO_IMPORT_MODE_3D_IGNOREGEOMETRY = 0x40000000,
			AUDIO_IMPORT_MODE_IGNORETAGS = 0x02000000,
			AUDIO_IMPORT_MODE_LOWMEM = 0x08000000,
			AUDIO_IMPORT_MODE_VIRTUAL_PLAYFROMSTART = 0x80000000
		};

		struct AssetManagerDesc
		{
			std::string mName = "UnNamed";
			ASSET_MANAGER_FLUSH_FLAGS mFlushFlagOnShutdown = ASSET_MANAGER_FLUSH_ALL;
			bool mSaveTexturePaths = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real texture name or not
			bool mSaveMeshPaths = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real mesh name or not
			bool mSaveMaterialsPaths = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real material name or not
		};

		class NEAPI AssetManager {
		public:
			AssetManager(AssetManagerDesc desc = AssetManagerDesc());
			~AssetManager();

			std::unordered_map<Uint32, Assets::Texture> mImportedTextures; //All loaded textures with their hashed names with crc32c (always saved)
			std::unordered_map<Uint32, Core::Path> mHashedTexturesPaths; //Real pre-hashed texture paths (conditionally saved see SaveTexturePaths)
			bool mSaveTexturesPaths = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real texture name or not

			std::unordered_map<Uint32, Assets::Mesh> mImportedMeshes; //All imported meshes with their hashed names with crc32c (always saved)
			std::unordered_map<Uint32, Core::Path> mHashedMeshesPaths; //Real pre-hashed meshes paths (conditionally saved see mSaveMeshPaths)
			bool mSaveMeshesPaths = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real meshes Paths or not

			std::unordered_map<Uint32, Assets::Material> mImportedMaterials; //All imported materials with their hashed names with crc32c (always saved)
			std::unordered_map<Uint32, Core::Path> mHashedMaterialsPaths; //Real pre-hashed materials paths (conditionally saved see mSaveMaterialPaths)
			bool mSaveMaterialsPaths = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real materials Paths or not

			bool mMultithreadMeshTextureLoading = true;

			Importers::TextureImporterDelegate mTextureImporter;
			Importers::MeshImporterDelegate mMeshImporter;

			//Note: Automatically called on Destruction
			void FlushContainers(ASSET_MANAGER_FLUSH_FLAGS = ASSET_MANAGER_FLUSH_ALL);

			Assets::Texture& Import(const Core::Path& Path, const Importers::TextureLoadingDesc& Desc = Importers::TextureLoadingDesc());
			Assets::Texture& Import(const Core::Path& Path, const Assets::TextureUsageType& type, const Importers::TextureLoadingDesc& Desc = Importers::TextureLoadingDesc());
			Assets::Texture& Import(const Assets::Image& Image, const Importers::TextureLoadingDesc& Desc = Importers::TextureLoadingDesc());


			Assets::AudioClip& Import(const Core::Path& Path, AUDIO_IMPORT_MODE mode = AUDIO_IMPORT_MODE_LOOP_OFF);

			std::tuple<Assets::Mesh*, Assets::Material*> Import(const Core::Path& Path, const Importers::MeshLoadingDesc& desc);
			
			bool DoesTextureExist(Uint32 hashedname, Assets::Texture* texture);

			//Order:  [+X (right)] [-X (left)] [+Y (top)] [-Y (bottom)] [+Z (front)] [-Z (back)]			
			std::array<Assets::Image, 6> LoadTextureCubeFromFile(const std::array<Core::Path, 6 >& Paths, const Importers::TextureLoadingDesc& Desc);
		private:
			AssetManagerDesc mDesc;
			Assets::Image TextureCube_Load(const Core::Path& Path, const Importers::TextureLoadingDesc& Desc);
		};
	}
}