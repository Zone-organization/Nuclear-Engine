#pragma once
#include <Assets/ImportingDescs.h>
#include <Assets/LoadingDescs.h>
#include <Assets/TextureDesc.h>

namespace Nuclear
{
	namespace Assets
	{
		class Texture;
		namespace Importers
		{
			class NEAPI TextureImporter
			{
			public:
				TextureImporter(TextureImporter const&) = delete;
				void operator=(TextureImporter const&) = delete;
				~TextureImporter();

				static TextureImporter& Get();

				bool Import(TextureData* data, IMAGE_EXTENSION extension, const Assets::TextureImportingDesc& desc);
				bool Export(const std::string& exportPath, TextureData* data, IMAGE_EXTENSION type);
				bool Load(const Assets::TextureLoadingDesc& Desc, TextureData* result);

				IMAGE_EXTENSION GetImageExtension(const std::string& filename);

				//Old:
				bool Load(const std::string& Path, Assets::TextureDesc* result ,const Assets::TextureImportingDesc& Desc);

				bool IsExtensionSupported(const std::string& extension);

			private:
				TextureImporter();

				bool FreeimageLoadMemory(IMAGE_EXTENSION type, Assets::TextureDesc* result, const Assets::TextureImportingDesc& Desc);
			};
		}
	}
}