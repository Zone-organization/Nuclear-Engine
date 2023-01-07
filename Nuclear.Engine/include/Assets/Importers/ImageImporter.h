#pragma once
#include <Assets/ImportingDescs.h>
#include <Assets/LoadingDescs.h>
#include <Assets/Image.h>

namespace Nuclear
{
	namespace Assets
	{
		class Image;
		namespace Importers
		{
			class NEAPI ImageImporter
			{
			public:
				ImageImporter(ImageImporter const&) = delete;
				void operator=(ImageImporter const&) = delete;
				~ImageImporter();

				static ImageImporter& GetInstance();

				bool Import(ImageData* data, IMAGE_EXTENSION extension, const Assets::ImageImportingDesc& desc);
				bool Export(const std::string& exportPath, ImageData* data, IMAGE_EXTENSION type);
				bool Load(const Assets::ImageLoadingDesc& Desc, ImageData* result);

				IMAGE_EXTENSION GetImageExtension(const std::string& filename);

				//Old:
				bool Load(const std::string& Path, Assets::ImageDesc* result ,const Assets::ImageImportingDesc& Desc);

				bool IsExtensionSupported(const std::string& extension);

			private:
				ImageImporter();

				bool FreeimageLoadMemory(IMAGE_EXTENSION type, Assets::ImageDesc* result, const Assets::ImageImportingDesc& Desc);
			};
		}
	}
}