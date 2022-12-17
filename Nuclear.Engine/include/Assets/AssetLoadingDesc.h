#pragma once
#include <Assets/ImageData.h>
#include <Diligent/Graphics/GraphicsEngine/interface/GraphicsTypes.h>
#include <string>
#include <tuple>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <vector>

namespace Nuclear
{
	namespace Assets
	{
		struct AssetLoadingDesc 
		{
			//Empty?
		};


		struct MeshLoadingDesc : public AssetLoadingDesc
		{
			bool SaveMaterialNames = true;
			bool LoadAnimation = true;
			bool LoadMaterial = true;
		};

		DILIGENT_TYPED_ENUM(TEXTURE_LOAD_MIP_FILTER, Uint8)
		{
			/// Default filter type: BOX_AVERAGE for UNORM/SNORM and FP formats, and
			/// MOST_FREQUENT for UINT/SINT formats.
			TEXTURE_LOAD_MIP_FILTER_DEFAULT = 0,

				/// 2x2 box average.
				TEXTURE_LOAD_MIP_FILTER_BOX_AVERAGE,

				/// Use the most frequent element from the 2x2 box.
				/// This filter does not introduce new values and should be used
				/// for integer textures that contain non-filterable data (e.g. indices).
				TEXTURE_LOAD_MIP_FILTER_MOST_FREQUENT
		};

		struct ImageLoadingDesc : public AssetLoadingDesc
		{
			Uint32 mHashedPath;

			USAGE mUsage;

			BIND_FLAGS mBindFlags;

			Uint32 mMipLevels;

			CPU_ACCESS_FLAGS mCPUAccessFlags;

			bool mIsSRGB;

			bool mGenerateMips;

			bool mFlipY_Axis;

			bool mLoadFromMemory;

			RESOURCE_DIMENSION mType;

			void* mMemData;

			Uint32 mMemSize;

			float AlphaCutoff = 0;

			// Coarse mip filter type, see Diligent::TEXTURE_LOAD_MIP_FILTER.
			TEXTURE_LOAD_MIP_FILTER MipFilter = TEXTURE_LOAD_MIP_FILTER_DEFAULT;

			//TEXTURE_FORMAT mFormat;

			ImageLoadingDesc() :
				mUsage(USAGE_IMMUTABLE),
				mBindFlags(BIND_SHADER_RESOURCE),
				mMipLevels(0),
				mCPUAccessFlags(CPU_ACCESS_NONE),
				mIsSRGB(false),
				mGenerateMips(true),
				mFlipY_Axis(true),
				/*	mFormat(TEX_FORMAT_UNKNOWN),*/
				mLoadFromMemory(false),
				mMemData(nullptr),
				mMemSize(0),
				mType(RESOURCE_DIM_TEX_2D)
			{}
		};

		struct FontLoadingDesc : public AssetLoadingDesc
		{
			Uint32 Width = 0;
			Uint32 Height = 48;

		};

		enum class ShaderType
		{	_3DRendering,
			PostFX,
			Unknown
		};

		struct ShaderLoadingDesc : public AssetLoadingDesc
		{
			std::string mShaderName = "UnNamed";
			ShaderType mType = ShaderType::Unknown;

			std::vector<std::string> mDefines;
		};

		struct ScriptLoadingDesc : public AssetLoadingDesc
		{

			std::string mScriptFullName;
			bool mClassNameFromPath = true;
		};
		struct SceneLoadingDesc : public AssetLoadingDesc
		{
			std::string mName;
		};
	}
}