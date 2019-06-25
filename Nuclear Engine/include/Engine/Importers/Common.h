#pragma once
#include <Engine/Assets/Texture.h>
#include <Engine/Assets/Mesh.h>
#include <Engine/Assets/Material.h>
#include <Engine/Assets/Image.h>
#include <Base\Utilities\Delegate.h>
#include <string>
#include <tuple>

namespace NuclearEngine
{
	namespace Managers { class AssetManager; }

	namespace Importers
	{
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

		struct MeshImporterDesc
		{
			std::string mPath;
			MeshLoadingDesc mMeshDesc;
			Managers::AssetManager* mManager = nullptr;
		};

		typedef Utilities::Delegate<std::tuple<std::vector<Assets::Mesh::SubMesh>, Assets::Material>(const MeshImporterDesc& desc)> MeshImporterDelegate;
		typedef Utilities::Delegate<Assets::Image& (const std::string& Path, const TextureLoadingDesc& Desc)> TextureImporterDelegate;
	}
}
