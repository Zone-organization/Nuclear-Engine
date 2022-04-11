#pragma once
#ifdef EXPOSE_FREEIMAGE_IMPORTER
#include <Engine/Importers/Common.h>
#include <Engine/Assets/ImageData.h>

namespace Nuclear
{
	namespace Importers
	{
		Assets::ImageData FreeImageLoad(const std::string& Path, const Importers::ImageLoadingDesc & Desc);
	}
}
#endif