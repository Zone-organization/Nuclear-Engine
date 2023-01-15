#pragma once
#include <Assets/LoadingDescs.h>
#include <Diligent/Graphics/GraphicsEngine/interface/GraphicsTypes.h>
#include <string>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <vector>

namespace Nuclear
{
	namespace Assets
	{

		struct AudioClipImportingDesc
		{
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

			AUDIO_IMPORT_MODE mMode = AUDIO_IMPORT_MODE_LOOP_OFF;
		};
		struct ModelImportingDesc
		{
			bool SaveMaterialNames = true;
			bool LoadMesh = true;
			bool LoadAnimation = true;
			bool LoadMaterialInfo = true;
		};

		struct TextureImportingDesc
		{
			Diligent::USAGE mUsage;

			Diligent::BIND_FLAGS mBindFlags;

			Uint32 mMipLevels;

			Diligent::CPU_ACCESS_FLAGS mCPUAccessFlags;

			bool mIsSRGB;

			bool mGenerateMips;

			bool mFlipY_Axis;

			bool mLoadFromMemory;

			Diligent::RESOURCE_DIMENSION mType;

			void* mMemData;

			Uint32 mMemSize;

			IMAGE_EXTENSION mExportExtension;

			bool mAsyncImporting;

			bool mLoadOnly;

			TextureImportingDesc() :
				mUsage(Diligent::USAGE_IMMUTABLE),
				mBindFlags(Diligent::BIND_SHADER_RESOURCE),
				mMipLevels(0),
				mCPUAccessFlags(Diligent::CPU_ACCESS_NONE),
				mIsSRGB(false),
				mGenerateMips(true),
				mFlipY_Axis(true),
				mLoadFromMemory(false),
				mMemData(nullptr),
				mMemSize(0),
				mType(Diligent::RESOURCE_DIM_TEX_2D),
				mExportExtension(IMAGE_EXTENSION_DDS),
				mAsyncImporting(true),
				mLoadOnly(true)
			{
			}
		};

		struct FontImportingDesc
		{
			Uint32 Width = 0;
			Uint32 Height = 48;

		};

		enum class ShaderType
		{	_3DRendering,
			PostFX,
			Unknown
		};

		struct ShaderImportingDesc
		{
			std::string mShaderName = "UnNamed";
			ShaderType mType = ShaderType::Unknown;

			std::vector<std::string> mDefines;
			std::vector<std::string> mExcludedVariants;
		};

		struct ScriptImportingDesc
		{

			std::string mScriptFullName;
			bool mClassNameFromPath = true;
		};
		struct SceneImportingDesc
		{
			std::string mName;
		};
	}
}