#pragma once
#include <Base\Math\Math.h>
#include <Engine/Assets/Image.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		enum TextureUsageType : Uint8
		{
			Diffuse = 0,
			Albedo = 0,

			Specular = 1,
			Metallic = 1,

			Normal = 2,

			Roughness = 3,
			AO = 4,

			Unknown = 255
		};

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