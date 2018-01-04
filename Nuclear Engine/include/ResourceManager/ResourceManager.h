#pragma once
#include <NE_Common.h>
#include <API\Texture.h>
#include <API\Color.h>
#include <array>
namespace NuclearEngine {
	class NEAPI ResourceManager {
	public:
		static API::Texture_Data LoadTextureFromFile(const char* filename, const API::Texture_Desc& Desc);

		/*
		Order:
		 +X (right)
		 -X (left)
		 +Y (top)
		 -Y (bottom)
		 +Z (front)
		 -Z (back)
		*/
		static  std::array<API::Texture_Data, 6> LoadTextureCubeFromFile(const std::array<std::string, 6 >& filenames, const API::Texture_Desc& Desc);
	};
}