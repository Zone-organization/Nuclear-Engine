#include "AssetManager\AssetManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\Thirdparty\stb_image.h"
#include <Components\Model.h>
#include "AssimpImporter.h"

namespace NuclearEngine {
	
	bool AssetManager::LoadModel(std::string Path, Components::Model * model)
	{
		Internal::AssimpImporter importer;
		return importer.Load(Path, model);
	}
	API::Texture_Data AssetManager::LoadTextureFromFile(std::string Path, const API::Texture_Desc & Desc)
	{
		int req_c;
		switch (Desc.Format)
		{
		case API::Format::R8:		req_c = 1; break;
		case API::Format::R8G8:		req_c = 2; break;
		case API::Format::R8G8B8:	req_c = 3; break;
		case API::Format::R8G8B8A8:	req_c = 4; break;
		default:					req_c = 4; break;
		}

		API::Texture_Data Data;

		stbi_set_flip_vertically_on_load(true); // Note:FLip Texture in Y - axis while loading
		Data.Img_Data_Buf = stbi_load(Path.c_str(), &Data.Width, &Data.Height, &Data.Components_Number, req_c);

		if (Data.Img_Data_Buf == NULL)
		{
			Log->Error(std::string("[AssetManager] Failed To Load Texture: " + Path + '\n'));

		}

		Log->Info(std::string("[AssetManager] Loaded Texture: " + Path + '\n'));

		return Data;
	}

	void AssetManager::CreateTextureFromFile(std::string Path, API::Texture * texture, const API::Texture_Desc & Desc)
	{
		API::Texture::Create(texture, AssetManager::LoadTextureFromFile(Path, Desc), Desc);
	}

	API::Texture_Data LoadTextureFromFile_NoFlip(std::string Path, const API::Texture_Desc & Desc)
	{

		int req_c;
		switch (Desc.Format)
		{
		case API::Format::R8:		req_c = 1; break;
		case API::Format::R8G8:		req_c = 2; break;
		case API::Format::R8G8B8:	req_c = 3; break;
		case API::Format::R8G8B8A8:	req_c = 4; break;
		default:					req_c = 4; break;
		}

		API::Texture_Data Data;

		Data.Img_Data_Buf = stbi_load(Path.c_str(), &Data.Width, &Data.Height, &Data.Components_Number, req_c);
		if (Data.Img_Data_Buf == NULL)
		{
			Log->Error(std::string("[AssetManager] Failed To Load Texture: " + Path + '\n'));

		}

		Log->Info(std::string("[AssetManager] Loaded Texture: " + Path + '\n'));

		return Data;
	}
	std::array<API::Texture_Data, 6> AssetManager::LoadTextureCubeFromFile(const std::array<std::string, 6>& Paths, const API::Texture_Desc& Desc)
	{
		API::Texture_Data data1, data2, data3, data4, data5, data6;

		//URGENT FIX SKYBOX VERTICES
		data1 = LoadTextureFromFile_NoFlip(Paths.at(0), Desc);
		data2 = LoadTextureFromFile_NoFlip(Paths.at(1), Desc);
		data3 = LoadTextureFromFile_NoFlip(Paths.at(2), Desc);
		data4 = LoadTextureFromFile_NoFlip(Paths.at(3), Desc);
		data5 = LoadTextureFromFile_NoFlip(Paths.at(4), Desc);
		data6 = LoadTextureFromFile_NoFlip(Paths.at(5), Desc);

		std::array<API::Texture_Data, 6> result = { data1, data2, data3, data4, data5, data6 };

		return result;
	}
}