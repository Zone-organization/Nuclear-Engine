#pragma once
#include <Base\Math\Math.h>
#include <Diligent/Graphics/GraphicsEngine/interface/TextureView.h>
#include <Engine/Assets/Asset.h>
#include <vector>

namespace NuclearEngine {
	namespace Assets {
		enum TextureUsageType : Uint8
		{
			Diffuse = 1,
			Specular = 2,
			Normal = 3,

			Unknown = 255
		};

		class NEAPI Texture : public Asset
		{
		public:
			Texture();
			~Texture();

			Uint8 GetUsageType();
			void SetUsageType(Uint8 type);
			void SetUsageType(TextureUsageType type);

			ITextureView*  mTexture = nullptr;

		private:
			Uint8 mUsageType;
		};
	}
}