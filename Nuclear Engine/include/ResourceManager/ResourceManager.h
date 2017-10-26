#pragma once
#include <NE_Common.h>
#include <API\Texture2D.h>

namespace NuclearEngine {
	class NEAPI ResourceManager {
	public:
		static Texture_Data LoadTextureFromFile(const char* filename, const Texture_Desc& Desc);
	};
}