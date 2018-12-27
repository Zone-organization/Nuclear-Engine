#pragma once
#include <Engine\Managers\AssetManager.h>

#ifdef EXPOSE_STB_IMAGE_IMPORTER
namespace NuclearEngine
{
	namespace Importers
	{
		static Assets::Image& LoadTex_STB(const std::string& Path, const TextureLoadingDesc & Desc);
	}
}
#endif