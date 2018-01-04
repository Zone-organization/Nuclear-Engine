#include "ResourceManager\ResourceManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\Thirdparty\stb_image.h"

namespace NuclearEngine {
	API::Texture_Data ResourceManager::LoadTextureFromFile(const char * filename, const API::Texture_Desc & Desc)
	{
		
		int req_c;
		switch (Desc.Format)   //Correctly Set The Required Componenets for stb_image
		{
		case API::Format::R8:
			req_c = 1;
		case API::Format::R8G8:
			req_c = 2;
		case API::Format::R8G8B8:
			req_c = 3;
		case API::Format::R8G8B8A8:
			req_c = 4;
		default:
			req_c = 4;
		}

		API::Texture_Data Data;

		stbi_set_flip_vertically_on_load(true); // Note:FLip Texture in Y - axis while loading
		Data.Img_Data_Buf = stbi_load(filename, &Data.Width, &Data.Height, &Data.Components_Number, req_c);

		if (Data.Img_Data_Buf == NULL)
		{
			Log->Error(std::string("[ResourceManager] Failed To Load Texture: " + std::string(filename) + '\n'));

		}

		Log->Info(std::string("[ResourceManager] Loaded Texture: " + std::string(filename) + '\n'));

		return Data;
	}

	API::Texture_Data LoadTextureFromFile_NoFlip(const char * filename, const API::Texture_Desc & Desc)
	{
		

		int req_c;
		switch (Desc.Format)   //Correctly Set The Required Componenets for stb_image
		{
		case API::Format::R8:
			req_c = 1;
		case API::Format::R8G8:
			req_c = 2;
		case API::Format::R8G8B8:
			req_c = 3;
		case API::Format::R8G8B8A8:
			req_c = 4;
		default:
			req_c = 4;
		}

		API::Texture_Data Data;

		Data.Img_Data_Buf = stbi_load(filename, &Data.Width, &Data.Height, &Data.Components_Number, req_c);
		if (Data.Img_Data_Buf == NULL)
		{
			Log->Error(std::string("[ResourceManager] Failed To Load Texture: " + std::string(filename) + '\n'));

		}

		Log->Info(std::string("[ResourceManager] Loaded Texture: " + std::string(filename) + '\n'));

		return Data;
	}
	std::array<API::Texture_Data, 6> ResourceManager::LoadTextureCubeFromFile(const std::array<std::string, 6>& filenames, const API::Texture_Desc& Desc)
	{
		API::Texture_Data data1, data2, data3, data4, data5, data6;

		//URGENT FIX SKYBOX VERTICES
		data1 = LoadTextureFromFile_NoFlip(filenames.at(0).c_str(), Desc);
		data2 = LoadTextureFromFile_NoFlip(filenames.at(1).c_str(), Desc);
		data3 = LoadTextureFromFile_NoFlip(filenames.at(2).c_str(), Desc);
		data4 = LoadTextureFromFile_NoFlip(filenames.at(3).c_str(), Desc);
		data5 = LoadTextureFromFile_NoFlip(filenames.at(4).c_str(), Desc);
		data6 = LoadTextureFromFile_NoFlip(filenames.at(5).c_str(), Desc);

		std::array<API::Texture_Data, 6> result = { data1, data2, data3, data4, data5, data6 };

		return result;
	}
}