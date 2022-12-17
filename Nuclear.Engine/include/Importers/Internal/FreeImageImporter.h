#pragma once
#include <Assets/AssetLoadingDesc.h>
#include <Assets/ImageData.h>

namespace Nuclear
{
	namespace Importers
	{
		namespace Internal
		{
			class NEAPI FreeImageImporter
			{
			public:
				FreeImageImporter(FreeImageImporter const&) = delete;
				void operator=(FreeImageImporter const&) = delete;
				~FreeImageImporter();

				static FreeImageImporter& GetInstance();

				Assets::ImageData Load(const std::string& Path, const Assets::ImageLoadingDesc& Desc);

				bool IsExtensionSupported(const std::string& extension);

			private:
				FreeImageImporter();
			};
		}
	}
}