#pragma once
#include <Assets/ImportingDescs.h>
#include <Assets/ImageData.h>

namespace Nuclear
{
	namespace Assets
	{
		namespace Importers
		{
			class NEAPI FreeImageImporter
			{
			public:
				FreeImageImporter(FreeImageImporter const&) = delete;
				void operator=(FreeImageImporter const&) = delete;
				~FreeImageImporter();

				static FreeImageImporter& GetInstance();

				Assets::ImageData Load(const std::string& Path, const Assets::ImageImportingDesc& Desc);
				bool Load(const std::string& Path, Assets::ImageData* result ,const Assets::ImageImportingDesc& Desc);

				bool IsExtensionSupported(const std::string& extension);

			private:
				FreeImageImporter();
			};
		}
	}
}