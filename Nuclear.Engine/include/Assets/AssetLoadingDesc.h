#pragma once
#include <Assets/ImageData.h>
#include <Diligent/Graphics/GraphicsEngine/interface/GraphicsTypes.h>
#include <string>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <vector>
#include <Graphics/TextureUsageType.h>

namespace Nuclear
{
	namespace Assets
	{
		class Mesh;
		class MaterialData;
		class Animations;

		struct AssetLoadingDesc 
		{
			//Empty?
		};


		struct ModelLoadingDesc : public AssetLoadingDesc
		{
			bool SaveMaterialNames = true;
			bool LoadMesh = true;
			bool LoadAnimation = true;
			bool LoadMaterialData = true;
		};
		struct ImportedModel
		{
			Assets::Mesh* pMesh = nullptr;
			Assets::MaterialData* pMaterialData = nullptr;
			Assets::Animations* pAnimations = nullptr;
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

		struct TextureLoadingDesc : public AssetLoadingDesc
		{
			TextureLoadingDesc()
			{
				mImageDesc = ImageLoadingDesc();
				mType = Graphics::TextureUsageType::Unknown;
				mHashedPath = 0;
			}

			TextureLoadingDesc(Graphics::TextureUsageType type)
			{
				mImageDesc = ImageLoadingDesc();
				mType = type;
				mHashedPath = 0;
			}

			TextureLoadingDesc(const ImageLoadingDesc& desc, Graphics::TextureUsageType type)
			{
				mImageDesc = desc;
				mType = type;
				mHashedPath = 0;
			}

			
			TextureLoadingDesc(const ImageLoadingDesc& desc, Graphics::TextureUsageType type, Uint32 hashedpath)
			{
				mImageDesc = desc;
				mType = type;
				mHashedPath = hashedpath;
			}
			ImageLoadingDesc mImageDesc;
			Graphics::TextureUsageType mType;
			Uint32 mHashedPath = 0;
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