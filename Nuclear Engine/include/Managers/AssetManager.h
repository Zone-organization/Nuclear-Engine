#pragma once
#include <Graphics/API/Texture.h>
#include <Graphics/Color.h>
#include <unordered_map>
#include <array>
#include <string>
namespace NuclearEngine {
	namespace Assets
	{
		class Mesh;
	}
	namespace Managers
	{

		struct MeshLoadingDesc
		{
			bool UseTexCoords = true;
			bool UseNormals = true;
			bool UseTangents = false;
			bool LoadDiffuseTextures = true;
			bool LoadSpecularTextures = false;
			bool LoadNormalTextures = false;
		};

		class NEAPI AssetManager {
		public:
			//Public Members
			//All loaded textures with their hashed names with crc32c (always saved)
			static std::unordered_map<Uint32, Graphics::API::Texture> mLoadedTextures;
			//Real pre-hashed texture names with paths (conditionally saved see SaveTextureNames)
			static std::unordered_map<Uint32,std::string> mHashedTexturesNames;
			//tells the asset manager whether to store the real texture name or not
			static bool mSaveTextureNames;

			//Methods
			static void Initialize(bool SaveTextureNames = false);
			static void ReleaseAllTextures();
			static bool LoadModel(std::string Path, Assets::Mesh *model, const MeshLoadingDesc& = MeshLoadingDesc());
			static void CreateTextureFromFile(std::string Path, Graphics::API::Texture* texture, const Graphics::API::Texture_Desc& Desc);
			static Graphics::API::Texture CreateTextureFromFile(std::string Path, const Graphics::API::Texture_Desc& Desc);

			//Order:  [+X (right)] [-X (left)] [+Y (top)] [-Y (bottom)] [+Z (front)] [-Z (back)]			
			static std::array<Graphics::API::Texture_Data, 6> LoadTextureCubeFromFile(const std::array<std::string, 6 >& Paths, const Graphics::API::Texture_Desc& Desc);
		private:
			static Graphics::API::Texture_Data LoadTextureFromFile(std::string Path, Uint32 Hashedname, const Graphics::API::Texture_Desc& Desc);

		};
	}
}