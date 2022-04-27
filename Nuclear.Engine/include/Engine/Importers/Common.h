#pragma once
#include <Engine/Assets/ImageData.h>
#include <Core\Utilities\Delegate.h>
#include <Diligent/Graphics/GraphicsEngine/interface/GraphicsTypes.h>
#include <string>
#include <tuple>

namespace Nuclear
{
	namespace Assets {
		class Mesh;
		class Material;
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

		struct MeshImporterDesc
		{
			const char* mPath;
			MeshLoadingDesc mMeshDesc = MeshLoadingDesc();
			Managers::AssetManager* mManager = nullptr;
		};

		typedef Utilities::Delegate<bool(const MeshImporterDesc& desc, Assets::Mesh*, Assets::Material*, Assets::Animations*)> MeshImporterDelegate;
		typedef Utilities::Delegate<Assets::ImageData(const std::string& Path, const ImageLoadingDesc& Desc)> ImageImporterDelegate;
	}
}
