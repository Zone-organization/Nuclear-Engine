#pragma once
#include <Engine/Assets/ImageData.h>
#include <Core\Utilities\Delegate.h>
#include <Diligent/Graphics/GraphicsEngine/interface/GraphicsTypes.h>
#include <string>
#include <tuple>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <vector>

namespace Nuclear
{
	namespace Assets {
		class Mesh;
		class Material;
		class MaterialData;
		class Animations;
	}
	namespace Managers { class AssetManager; }

	namespace Importers
	{
		enum MeshTextureType
		{
			NONE = 0x0,

			ALBEDO = 0x1,

			SPECULAR = 0x2,

			AMBIENT = 0x3,

			EMISSIVE = 0x4,

			HEIGHT = 0x5,

			NORMALS = 0x6,

			SHININESS = 0x7,

			OPACITY = 0x8,

			DISPLACEMENT = 0x9,

			LIGHTMAP = 0xA,

			REFLECTION = 0xB,

			UNKNOWN = 0xC,

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

		struct MeshLoadingDesc
		{
			bool SaveMaterialNames = true;
			bool LoadAnimation = true;
			bool LoadMaterial = true;
		};
		struct ImageLoadingDesc
		{
			std::string mPath;

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

			float AlphaCutoff  = 0;

			// Coarse mip filter type, see Diligent::TEXTURE_LOAD_MIP_FILTER.
			TEXTURE_LOAD_MIP_FILTER MipFilter = TEXTURE_LOAD_MIP_FILTER_DEFAULT;

			//TEXTURE_FORMAT mFormat;

			ImageLoadingDesc() :
				mPath(""),
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
		struct FontLoadingDesc
		{
			Uint32 Width = 0;
			Uint32 Height = 48;

		};

		enum class ShaderType
		{
			_3DRendering,
			PostFX,
			Unknown
		};

		struct ShaderLoadingDesc
		{
			std::string mShaderName = "UnNamed";
			ShaderType mType = ShaderType::Unknown;

			std::vector<std::string> mDefines;
		};

		struct MeshImporterDesc
		{
			const char* mPath;
			MeshLoadingDesc mMeshDesc = MeshLoadingDesc();
			Managers::AssetManager* mManager = nullptr;
		};

		typedef Utilities::Delegate<bool(const MeshImporterDesc& desc, Assets::Mesh*, Assets::MaterialData*, Assets::Animations*)> MeshImporterDelegate;
		typedef Utilities::Delegate<Assets::ImageData(const std::string& Path, const ImageLoadingDesc& Desc)> ImageImporterDelegate;
	}
}
