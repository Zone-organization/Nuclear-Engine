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
		Graphics::API::Texture_Data AssetManager::LoadTextureFromFile(std::string Path, Uint32 Hashedname, const Graphics::API::Texture_Desc & Desc, bool flip)
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

			stbi_set_flip_vertically_on_load(flip);
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

		bool AssetManager::CreateTextureFromFile(std::string Path, Graphics::API::Texture * ptexture, const Graphics::API::Texture_Desc & Desc, bool flip)
		{
			auto hashedname = Utilities::Hash(Path);
			if (DoesTextureExist(hashedname, ptexture))
			{
				return true;
			}
			else 
			{
				Graphics::API::Texture texture;
				auto data = AssetManager::LoadTextureFromFile(Path, hashedname, Desc, flip);
				Graphics::API::Texture::Create(&texture, data, Desc);

				if (data.Valid)
				{
					mLoadedTextures[hashedname] = texture;
					ptexture = &texture;
				}
				return data.Valid;
			}
			return false;
		}

		Graphics::API::Texture* AssetManager::CreateTextureFromFile(std::string Path, const Graphics::API::Texture_Desc & Desc, bool flip)
		{
			Graphics::API::Texture *texture;
			CreateTextureFromFile(Path,texture,Desc, flip);
			return texture;
		}
		bool AssetManager::DoesTextureExist(Uint32 hashedname, Graphics::API::Texture* texture)
		{
			//Check if Texture has been loaded before
			auto it = mLoadedTextures.find(hashedname);
			if (it != mLoadedTextures.end())
			{
				texture = &it->second;
				return true;
			}
			texture = nullptr;
			return false;
		}
		std::array<Graphics::API::Texture_Data, 6> AssetManager::LoadTextureCubeFromFile(const std::array<std::string, 6>& Paths, const Graphics::API::Texture_Desc& Desc)
		{
			Graphics::API::Texture_Data data1, data2, data3, data4, data5, data6;

			data1 = LoadTextureFromFile(Paths.at(0), Utilities::Hash(Paths.at(0)), Desc,false);
			data2 = LoadTextureFromFile(Paths.at(1), Utilities::Hash(Paths.at(1)), Desc,false);
			data3 = LoadTextureFromFile(Paths.at(2), Utilities::Hash(Paths.at(2)), Desc, false);
			data4 = LoadTextureFromFile(Paths.at(3), Utilities::Hash(Paths.at(3)), Desc, false);
			data5 = LoadTextureFromFile(Paths.at(4), Utilities::Hash(Paths.at(4)), Desc, false);
			data6 = LoadTextureFromFile(Paths.at(5), Utilities::Hash(Paths.at(5)), Desc, false);
			
			std::array<Graphics::API::Texture_Data, 6> result = { data1, data2, data3, data4, data5, data6 };

			return result;
		}
	}
}