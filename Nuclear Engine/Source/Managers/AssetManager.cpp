#include "Managers\AssetManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\Thirdparty\stb_image.h"
#include <XAsset\ModelAsset.h>
#include "AssimpImporter.h"

namespace NuclearEngine {
	namespace Managers {
		bool AssetManager::LoadModel(std::string Path, XAsset::ModelAsset * model, const ModelLoadingDesc& desc)
		{
			Internal::AssimpImporter importer;
			return importer.Load(Path, model, desc);
		}
		Graphics::API::Texture_Data AssetManager::LoadTextureFromFile(std::string Path, const Graphics::API::Texture_Desc & Desc)
		{
			int req_c;
			switch (Desc.Format)
			{
			case Graphics::API::Format::R8_UNORM:	
				req_c = 1; break;
			case Graphics::API::Format::R8G8_UNORM:
				req_c = 2; break;
			case Graphics::API::Format::R8G8B8_UNORM:
				req_c = 3; break;
			case Graphics::API::Format::R8G8B8A8_UNORM:
			case Graphics::API::Format::R8G8B8A8_UNORM_SRGB:
				req_c = 4; break;
			default:					req_c = 4; break;
			}

			Graphics::API::Texture_Data Data;

			stbi_set_flip_vertically_on_load(true); // Note:FLip Texture in Y - axis while loading
			Data.Img_Data_Buf = stbi_load(Path.c_str(), &Data.Width, &Data.Height, &Data.Components_Number, req_c);

			if (Data.Img_Data_Buf == NULL)
			{
				Log.Error(std::string("[AssetManager] Failed To Load Texture: " + Path + '\n'));

			}

			Log.Info(std::string("[AssetManager] Loaded Texture: " + Path + '\n'));

			return Data;
		}

		void AssetManager::CreateTextureFromFile(std::string Path, Graphics::API::Texture * texture, const Graphics::API::Texture_Desc & Desc)
		{
			Graphics::API::Texture::Create(texture, AssetManager::LoadTextureFromFile(Path, Desc), Desc);
		}

		Graphics::API::Texture_Data LoadTextureFromFile_NoFlip(std::string Path, const Graphics::API::Texture_Desc & Desc)
		{

			int req_c;
			switch (Desc.Format)
			{
			case Graphics::API::Format::R8_UNORM:	
				req_c = 1; break;
			case Graphics::API::Format::R8G8_UNORM:
				req_c = 2; break;
			case Graphics::API::Format::R8G8B8_UNORM:
				req_c = 3; break;
			case Graphics::API::Format::R8G8B8A8_UNORM:
			case Graphics::API::Format::R8G8B8A8_UNORM_SRGB:
				req_c = 4; break;
			default:					req_c = 4; break;
			}

			Graphics::API::Texture_Data Data;
			stbi_set_flip_vertically_on_load(false); // Note:FLip Texture in Y - axis while loading

			Data.Img_Data_Buf = stbi_load(Path.c_str(), &Data.Width, &Data.Height, &Data.Components_Number, req_c);
			if (Data.Img_Data_Buf == NULL)
			{
				Log.Error(std::string("[AssetManager] Failed To Load Texture: " + Path + '\n'));

			}

			Log.Info(std::string("[AssetManager] Loaded Texture: " + Path + '\n'));

			return Data;
		}
		std::array<Graphics::API::Texture_Data, 6> AssetManager::LoadTextureCubeFromFile(const std::array<std::string, 6>& Paths, const Graphics::API::Texture_Desc& Desc)
		{
			Graphics::API::Texture_Data data1, data2, data3, data4, data5, data6;

			data1 = LoadTextureFromFile_NoFlip(Paths.at(0), Desc);
			data2 = LoadTextureFromFile_NoFlip(Paths.at(1), Desc);
			data3 = LoadTextureFromFile_NoFlip(Paths.at(2), Desc);
			data4 = LoadTextureFromFile_NoFlip(Paths.at(3), Desc);
			data5 = LoadTextureFromFile_NoFlip(Paths.at(4), Desc);
			data6 = LoadTextureFromFile_NoFlip(Paths.at(5), Desc);

			std::array<Graphics::API::Texture_Data, 6> result = { data1, data2, data3, data4, data5, data6 };

			return result;
		}
	}
}