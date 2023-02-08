#pragma once
#include <Assets/LoadingDescs.h>
#include <Diligent/Graphics/GraphicsEngine/interface/GraphicsTypes.h>
#include <string>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <vector>
#include <Graphics/ShaderTypes.h>

namespace Nuclear
{
	namespace Assets
	{
		class Shader;
		struct CommonImportingOptions
		{
			///if this string left empty the engine will assign a default name as "Directoryname::AssetFilenamewithextension"
			///Example:
			///c:/assets/textures/rustediron/albedotex.png -> rustediron_albedotex.png
			std::string mAssetName = "";

			///if this path is invalid the engine will import to default asset folder in assetlibrary path
			///Path to directory only
			Core::Path mExportPath;


			bool mChangeExportedName = true;

			//No exporting
			bool mLoadOnly = false;
						
			bool mAsyncImport = true;
		};
		struct AudioClipImportingDesc
		{
			CommonImportingOptions mCommonOptions;

			bool mLoop = false;
			bool mIs3D = false;
			Uint32 mBitDepth = 16; 
		};
		struct MeshImportingDesc
		{
			CommonImportingOptions mCommonOptions;

			Shader* pMaterialShader = nullptr;

			bool mSaveMaterialNames = true;
			bool mImportMesh = true;
			bool mImportAnimations = true;
			bool mImportMaterial = true;

			bool mExportMaterial = true;
		};

		struct MaterialImportingDesc
		{
			CommonImportingOptions mCommonOptions;
		};

		struct TextureImportingDesc
		{
			CommonImportingOptions mCommonOptions;

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

			bool mEngineAllocMem;

			IMAGE_EXTENSION mExportExtension;

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
				mEngineAllocMem(false)
			{
			}
		};

		struct FontImportingDesc
		{
			Uint32 Width = 0;
			Uint32 Height = 48;

		};

		struct ShaderImportingDesc
		{
			CommonImportingOptions mCommonOptions;
			Graphics::ShaderType mType = Graphics::ShaderType::Unknown;

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