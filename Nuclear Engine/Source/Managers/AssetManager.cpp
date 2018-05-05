#include "Managers\AssetManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\Thirdparty\stb_image.h"
#include <Assets\Mesh.h>
#include "AssimpImporter.h"
#include <Utilities\Hash.h>
#include <sstream>
#include <iomanip>

namespace NuclearEngine {
	namespace Managers {
		std::unordered_map<Uint32, Graphics::API::Texture> AssetManager::mLoadedTextures = std::unordered_map<Uint32, Graphics::API::Texture>();
		std::unordered_map<Uint32, std::string> AssetManager::mHashedTexturesNames = std::unordered_map<Uint32, std::string>();
		bool AssetManager::mSaveTextureNames = false;
		void AssetManager::Initialize(bool SaveTextureNames)
		{
			mSaveTextureNames = SaveTextureNames;
		}
		void AssetManager::ReleaseAllTextures()
		{
			for (auto x : mLoadedTextures)
			{
				Graphics::API::Texture::Delete(&x.second);
			}
			mLoadedTextures.clear();
			mHashedTexturesNames.clear();
		}
		bool AssetManager::LoadModel(std::string Path, Assets::Mesh * model, const MeshLoadingDesc& desc)
		{
			Internal::AssimpImporter importer;
			return importer.Load(Path, model, desc);
		}
		template< typename T >
		std::string int_to_hex(T i)
		{
			std::stringstream stream;
			stream << "0x"
				<< std::setfill('0') << std::setw(sizeof(T) * 2)
				<< std::hex << i;
			return stream.str();
		}
		Graphics::API::Texture_Data AssetManager::LoadTextureFromFile(std::string Path, Uint32 Hashedname, const Graphics::API::Texture_Desc & Desc)
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
				Data.Valid = false;
				return Data;
			}

			Data.Valid = true;
			Data.HashedName = Hashedname;
	
			if (mSaveTextureNames)
			{
				mHashedTexturesNames[Hashedname] = Path;
			}
			Log.Info(std::string("[AssetManager] Loaded Texture: " + Path + " Hash: " + int_to_hex<Uint32>(Hashedname) +'\n'));

			return Data;
		}

		bool AssetManager::CreateTextureFromFile(std::string Path, Graphics::API::Texture * texture, const Graphics::API::Texture_Desc & Desc)
		{
			//Check if Texture has been loaded before
			auto hashedname = Utilities::Hash(Path);
			auto it = mLoadedTextures.find(hashedname);
			if (it != mLoadedTextures.end())
			{
				texture = &it->second;
				return true;
			}
			else
			{
				auto data = AssetManager::LoadTextureFromFile(Path, hashedname, Desc);
				Graphics::API::Texture::Create(texture, data, Desc);

				if (data.Valid)
				{
					mLoadedTextures[hashedname] = *texture;
				}
				return data.Valid;
			}
		}

		Graphics::API::Texture AssetManager::CreateTextureFromFile(std::string Path, const Graphics::API::Texture_Desc & Desc)
		{
			Graphics::API::Texture texture;
			CreateTextureFromFile(Path,&texture,Desc);
			return texture;
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