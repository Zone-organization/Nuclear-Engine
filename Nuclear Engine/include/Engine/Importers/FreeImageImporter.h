#pragma once
#ifdef EXPOSE_FREEIMAGE_IMPORTER
#include <Engine/Importers/Common.h>

namespace NuclearEngine
{
	namespace Importers
	{
		Assets::Image& FreeImageLoad(const std::string& Path, const Importers::TextureLoadingDesc & Desc);
	}
}
#endif