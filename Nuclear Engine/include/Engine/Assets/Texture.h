#pragma once
#include <Base\Math\Math.h>
#include <Diligent/Graphics/GraphicsEngine/interface/TextureView.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Common/interface/RefCntAutoPtr.h>
#include <Engine/Assets/Common.h>
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

		class NEAPI Texture : public Asset<Texture>
		{
		public:
			Texture();
			~Texture();

			Uint8 GetUsageType() const;
			void SetUsageType(Uint8 type);
			void SetUsageType(TextureUsageType type);

			RefCntAutoPtr<ITextureView>  mTextureView;

		private:
			Uint8 mUsageType;
		};
	}
}