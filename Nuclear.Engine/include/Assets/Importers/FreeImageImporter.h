#pragma once
#include <Assets/ImportingDescs.h>
#include <Assets/LoadingDescs.h>

namespace Nuclear
{
	namespace Assets
	{
		class Image;
		namespace Importers
		{
			class NEAPI FreeImageImporter
			{
			public:
				FreeImageImporter(FreeImageImporter const&) = delete;
				void operator=(FreeImageImporter const&) = delete;
				~FreeImageImporter();

				static FreeImageImporter& GetInstance();

				bool Import(const std::string& importPath, const std::string& exportPath, Assets::Image* image, const Assets::ImageImportingDesc& desc);
				bool Load(const Assets::ImageLoadingDesc& Desc);

				bool Load(const std::string& Path, Assets::ImageData* result ,const Assets::ImageImportingDesc& Desc);

				bool IsExtensionSupported(const std::string& extension);

			private:
				FreeImageImporter();

				bool FreeimageLoad(const std::string& Path, Assets::ImageData* result, const Assets::ImageImportingDesc& Desc);
			};
		}
	}
}