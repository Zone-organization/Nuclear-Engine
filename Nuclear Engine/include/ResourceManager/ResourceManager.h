#pragma once
#include <NE_Common.h>
#include <API\Texture2D.h>
#include <API\Color.h>
namespace NuclearEngine {
	class NEAPI ResourceManager {
	public:
		static Texture_Data LoadTextureFromFile(const char* filename, const Texture_Desc& Desc);
		static Texture_Data FillWithColor(API::Color color, int width,int height, const Texture_Desc& Desc);
	};
}