#pragma once
#ifdef EXPOSE_FREEIMAGE_IMPORTER
#include <Engine/Importers/Common.h>

namespace NuclearEngine
{
	namespace Importers
	{
		bool FreeImageLoad(const std::string& Path, const Importers::ImageLoadingDesc & Desc, Assets::Image* result);
	}
}
#endif