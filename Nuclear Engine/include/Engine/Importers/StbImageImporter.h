#pragma once
#include <Engine\Managers\AssetManager.h>

#ifdef EXPOSE_STB_IMAGE_IMPORTER
namespace NuclearEngine
{
	namespace Importers
	{
		static Assets::Image& StbImageLoad(const std::string& Path, const Managers::TextureLoadingDesc & Desc);
	}
}
#endif