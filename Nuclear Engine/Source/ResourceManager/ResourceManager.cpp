#include "ResourceManager\ResourceManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\Thirdparty\stb_image.h"

namespace NuclearEngine {
	Texture_Data ResourceManager::LoadTextureFromFile(const char * filename, const Texture_Desc & Desc)
	{
		
		int req_c;
		switch (Desc.Format)   //Correctly Set The Required Componenets for stb_image
		{
		case TextureFormat::R8:
			req_c = 1;
		case TextureFormat::R8G8:
			req_c = 2;
		case TextureFormat::R8G8B8:
			req_c = 3;
		case TextureFormat::R8G8B8A8:
			req_c = 4;
		default:
			req_c = 4;
		}

		Texture_Data Data;

		stbi_set_flip_vertically_on_load(true); // Note:FLip Texture in Y - axis while loading
		Data.databuf = stbi_load(filename, &Data.width, &Data.height, &Data.no_of_components, req_c);

		if (Data.databuf == NULL)
		{
			Log->Error(std::string("[ResourceManager] Failed To Load Texture: " + std::string(filename) + '\n'));

		}

		Log->Info(std::string("[ResourceManager] Loaded Texture: " + std::string(filename) + '\n'));

		return Data;
	}

	Texture_Data LoadTextureFromFile_NoFlip(const char * filename, const Texture_Desc & Desc)
	{
		
		int req_c;
		switch (Desc.Format)   //Correctly Set The Required Componenets for stb_image
		{
		case TextureFormat::R8:
			req_c = 1;
		case TextureFormat::R8G8:
			req_c = 2;
		case TextureFormat::R8G8B8:
			req_c = 3;
		case TextureFormat::R8G8B8A8:
			req_c = 4;
		default:
			req_c = 4;
		}

		Texture_Data Data;

		Data.databuf = stbi_load(filename, &Data.width, &Data.height, &Data.no_of_components, req_c);
		if (Data.databuf == NULL)
		{
			Log->Error(std::string("[ResourceManager] Failed To Load Texture: " + std::string(filename) + '\n'));

		}

		Log->Info(std::string("[ResourceManager] Loaded Texture: " + std::string(filename) + '\n'));

		return Data;
	}
	std::array<NuclearEngine::Texture_Data, 6> ResourceManager::LoadTextureCubeFromFile(const std::array<std::string, 6>& filenames, const Texture_Desc& Desc)
	{
		Texture_Data data1, data2, data3, data4, data5, data6;

		//URGENT FIX SKYBOX VERTICES
		data1 = LoadTextureFromFile_NoFlip(filenames.at(0).c_str(), Desc);
		data2 = LoadTextureFromFile_NoFlip(filenames.at(1).c_str(), Desc);
		data3 = LoadTextureFromFile_NoFlip(filenames.at(2).c_str(), Desc);
		data4 = LoadTextureFromFile_NoFlip(filenames.at(3).c_str(), Desc);
		data5 = LoadTextureFromFile_NoFlip(filenames.at(4).c_str(), Desc);
		data6 = LoadTextureFromFile_NoFlip(filenames.at(5).c_str(), Desc);

		std::array<NuclearEngine::Texture_Data, 6> result = { data1, data2, data3, data4, data5, data6 };

		return result;
	}

	Texture_Data ResourceManager::FillWithColor(API::Color color, int width, int height, const Texture_Desc & Desc)
	{
		Exceptions::NotImplementedException();
		int req_c;
		switch (Desc.Format)   //Correctly Set The Required Componenets for stb_image
		{
		case TextureFormat::R8:
			req_c = 1;
		case TextureFormat::R8G8:
			req_c = 2;
		case TextureFormat::R8G8B8:
			req_c = 3;
		case TextureFormat::R8G8B8A8:
			req_c = 4;
		default:
			req_c = 4;
		}

		Texture_Data Data;

	/*	std::fill(
			Data.databuf,
			Data.databuf,
			color);*/

		return Data;
	}
}