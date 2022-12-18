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
			~Texture();

			Uint8 GetUsageType() const;
			void SetUsageType(Uint8 type);
			void SetUsageType(TextureUsageType type);

			inline Assets::Image* GetImage() { return mTextureImage; }
			inline void SetImage(Assets::Image* image) { mTextureImage = image; }

			inline Uint32 GetName() { return 	mHashedName; }
			inline void SetName(Uint32 name) { mHashedName = name; }
			inline std::string GetStringName() { return mName; }

			inline void SetName(const std::string& str) {
				mHashedName = Utilities::Hash(str);
				mName = str;
			}

		protected:
			Uint32 mHashedName = 0;
			std::string mName;
			Assets::Image* mTextureImage;
			Uint8 mUsageType;
		};
	}
}