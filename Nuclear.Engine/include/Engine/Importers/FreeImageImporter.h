#pragma once
#include <Engine/Importers/Common.h>
#include <Engine/Assets/ImageData.h>

namespace Nuclear
{
	namespace Importers
	{
		class FreeImage
		{
		public:
			static void Initialize();

			static void Shutdown();

			static Assets::ImageData Load(const std::string& Path, const Importers::ImageLoadingDesc& Desc);

			static bool IsExtensionSupported(const std::string& extension);
		};
	}
}