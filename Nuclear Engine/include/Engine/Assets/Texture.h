#pragma once
#include <Base\Math\Math.h>
#include <Engine/Graphics/API/Texture.h>
#include <vector>
#include <string>

namespace NuclearEngine {
	namespace Assets {
		enum TextureUsageType : Uint8
		{
			Diffuse = 1,
			Specular = 2,
			Normal = 3
		};

		class NEAPI Texture
		{
		public:
			Texture();
			~Texture();

			Math::Vector2ui GetDimensions();
			unsigned int GetWidth();
			unsigned int GetHeight();
			Uint32 GetHashedName();
			Uint8 GetUsageType();

			void SetHashedName(Uint32 name);
			void SetName(const std::string& str);
			void SetUsageType(Uint8 type);
			void SetUsageType(TextureUsageType type);

			Graphics::API::Texture mTexture;

		private:
			Math::Vector2ui mDimensions;
			Uint32 mHashedName;
			Uint8 mUsageType;
		};
	}
}