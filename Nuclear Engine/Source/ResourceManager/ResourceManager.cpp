#include "ResourceManager\ResourceManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\Thirdparty\stb_image.h"

namespace NuclearEngine {
	Texture_Data ResourceManager::LoadTextureFromFile(const char * filename, const Texture_Desc & Desc)
	{
		Log->Info("[ResourceManager] Loading Image: ");
		Log->Info(filename);
		Log->Info(" \n");

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
			Log->Error("[ResourceManager] Texture File Loading Failed!\n");
		}
		return Data;
	}
}