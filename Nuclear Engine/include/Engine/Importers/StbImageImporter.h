#pragma once
#ifdef EXPOSE_STB_IMAGE_IMPORTER
#include <Engine/Importers/Common.h>

namespace NuclearEngine
{
	namespace Importers
	{
		Assets::Image& StbImageLoad(const std::string& Path, const Importers::TextureLoadingDesc & Desc);
	}
}
#endif