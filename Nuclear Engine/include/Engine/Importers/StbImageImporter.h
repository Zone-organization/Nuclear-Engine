#pragma once
#include <Engine/Importers/Importers.h>

#ifdef EXPOSE_STB_IMAGE_IMPORTER
namespace NuclearEngine
{
	namespace Importers
	{
		Assets::Image& StbImageLoad(const std::string& Path, const Importers::TextureLoadingDesc & Desc);
	}
}
#endif