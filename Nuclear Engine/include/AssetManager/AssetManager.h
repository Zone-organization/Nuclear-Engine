#pragma once
#include <NE_Common.h>
#include <API\Texture.h>
#include <API\Color.h>
#include <array>
#include <string>
namespace NuclearEngine {
	namespace Components {
		class Model;
	}
	class NEAPI AssetManager {
	public:
		static bool LoadModel(std::string Path, Components::Model* Desc);

		static API::Texture_Data LoadTextureFromFile(std::string Path, const API::Texture_Desc& Desc);
		static void CreateTextureFromFile(std::string Path, API::Texture* texture, const API::Texture_Desc& Desc);

		/*
		Order:
		 +X (right)
		 -X (left)
		 +Y (top)
		 -Y (bottom)
		 +Z (front)
		 -Z (back)
		*/
		static std::array<API::Texture_Data, 6> LoadTextureCubeFromFile(const std::array<std::string, 6 >& Paths, const API::Texture_Desc& Desc);
	};
}