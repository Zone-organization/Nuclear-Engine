#pragma once
#include <Base\Math\Math.h>
#include <Diligent/Graphics/GraphicsEngine/interface/TextureView.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Engine/Assets/Common.h>
#include <vector>

namespace NuclearEngine
{
	namespace Assets
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

		class NEAPI Texture : public Asset<Texture>
		{
		public:
			Texture();
			~Texture();

			Uint8 GetUsageType() const;
			void SetUsageType(Uint8 type);
			void SetUsageType(TextureUsageType type);

			RefCntAutoPtr<ITextureView> mTextureView;

		private:
			Uint8 mUsageType;
		};
	}
}