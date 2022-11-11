#pragma once
#include <Engine/Importers/Common.h>
#include <Engine\Assets\AssetLibrary.h>
//#include <Engine\Threading\TaskManager.h>

#include <FMOD/inc/fmod.hpp>
#include <array>

#define MAX_BONE_INFLUENCE 4

typedef struct FT_LibraryRec_* FT_Library;
namespace msdfgen { class FreetypeHandle; }
namespace Nuclear 
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
			std::string mName = "UnNamed AssetManager";
			ASSET_MANAGER_FLUSH_FLAGS mFlushFlagOnShutdown = ASSET_MANAGER_FLUSH_ALL;
			bool mSaveTexturePaths = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real texture name or not
			bool mSaveMeshPaths = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real mesh name or not
			bool mSaveMaterialsPaths = DEBUG_TRUE_BOOL; //tells the asset manager whether to store the real material name or not
		};

		class NEAPI AssetManager {
		public:
			AssetManager(AssetManagerDesc desc = AssetManagerDesc());
			~AssetManager();

			Assets::AssetLibrary mLibrary;

			bool mMultithreadMeshTextureLoading = true;

			Importers::ImageImporterDelegate mImageImporter;
			Importers::MeshImporterDelegate mMeshImporter;

			//Note: Automatically called on Destruction
			void FlushContainers(ASSET_MANAGER_FLUSH_FLAGS = ASSET_MANAGER_FLUSH_ALL);
			void Initialize(AssetManagerDesc desc = AssetManagerDesc());

			//Textures
			Graphics::Texture Import(const Core::Path& Path, const Importers::ImageLoadingDesc& Desc = Importers::ImageLoadingDesc(), const Graphics::TextureUsageType& type = Graphics::TextureUsageType::Unknown);
			Graphics::Texture Import(const Assets::ImageData& Imagedata, const Importers::ImageLoadingDesc& Desc = Importers::ImageLoadingDesc());
		    //Order:  [+X (right)] [-X (left)] [+Y (top)] [-Y (bottom)] [+Z (front)] [-Z (back)]			
			std::array<Assets::Image*, 6> LoadTextureCubeFromFile(const std::array<Core::Path, 6 >& Paths, const Importers::ImageLoadingDesc& Desc);
			Assets::Image* DoesImageExist(Uint32 hashedname);


			//Audio
			Assets::AudioClip* Import(const Core::Path& Path, AUDIO_IMPORT_MODE mode = AUDIO_IMPORT_MODE_LOOP_OFF);

			//Mesh & Material & Animation
			std::tuple<Assets::Mesh*, Assets::MaterialData*, Assets::Animations*> Import(const Core::Path& Path, const Importers::MeshLoadingDesc& desc);

			Assets::Font* Import(const Core::Path& Path, const Importers::FontLoadingDesc& desc);

			//Shader
			Assets::Shader* Import(const Core::Path& Path, const Importers::ShaderLoadingDesc& desc);

			//Assets::Script& ImportScript(const Core::Path& Path);

			static Graphics::Texture DefaultBlackTex;
			static Graphics::Texture DefaultGreyTex;
			static Graphics::Texture DefaultWhiteTex;

			static Graphics::Texture DefaultDiffuseTex;
			static Graphics::Texture DefaultSpecularTex;
			static Graphics::Texture DefaultNormalTex;

		//	Graphics::Texture SaveToImport(const Core::Path& Path, const Importers::ImageLoadingDesc& Desc = Importers::ImageLoadingDesc(), const Graphics::TextureUsageType& type = Graphics::TextureUsageType::Unknown);

		private:
			AssetManagerDesc mDesc;
			Assets::Image* TextureCube_Load(const Core::Path& Path, const Importers::ImageLoadingDesc& Desc);
			msdfgen::FreetypeHandle* FT_Handle;

		//	Threading::TaskManager ImageLoadingTMgr;
			Assets::AssetLibrary mSavedToImport;
			RefCntAutoPtr<IShaderSourceInputStreamFactory> pShaderSourceISFactory;
		};
	}
}