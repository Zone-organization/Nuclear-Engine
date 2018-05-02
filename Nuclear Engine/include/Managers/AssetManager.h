#pragma once
#include <Graphics/API/Texture.h>
#include <Graphics/Color.h>
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

			static std::vector<Graphics::API::Texture> mLoadedTextures;

			static bool LoadModel(std::string Path, Assets::Mesh* model, const MeshLoadingDesc& = MeshLoadingDesc());

			static Graphics::API::Texture_Data LoadTextureFromFile(std::string Path, const Graphics::API::Texture_Desc& Desc);
			static void CreateTextureFromFile(std::string Path, Graphics::API::Texture* texture, const Graphics::API::Texture_Desc& Desc);

			/*
			Order:
			 +X (right)
			 -X (left)
			 +Y (top)
			 -Y (bottom)
			 +Z (front)
			 -Z (back)
			*/
			static std::array<Graphics::API::Texture_Data, 6> LoadTextureCubeFromFile(const std::array<std::string, 6 >& Paths, const Graphics::API::Texture_Desc& Desc);
		};
	}
}