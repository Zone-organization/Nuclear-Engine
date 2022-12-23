#pragma once
#include <Math\Math.h>
#include <Assets/Image.h>
#include <Graphics/TextureUsageType.h>

namespace Nuclear
{
	namespace Graphics
	{
		class NEAPI Texture 
		{
		public:
			Texture();
			Texture(Assets::Image* image, TextureUsageType type);
			~Texture();

			Uint8 GetUsageType() const;
			void SetUsageType(Uint8 type);
			void SetUsageType(TextureUsageType type);

			void Set(Assets::Image* image, TextureUsageType type);

			Assets::Image* GetImage();
			void SetImage(Assets::Image* image);

			Uint32 GetName();
			std::string GetStringName();

			void SetName(const std::string& str);

		protected:
			Uint32 mHashedName = 0;
			std::string mName;
			Assets::Image* mTextureImage;
			Uint8 mUsageType;
		};
	}
}