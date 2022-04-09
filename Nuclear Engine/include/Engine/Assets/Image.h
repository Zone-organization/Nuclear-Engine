#pragma once
#include <Engine/Assets/Common.h>
#include <Base\Math\Math.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Engine/Importers/Common.h>

namespace NuclearEngine
{
	namespace Assets
	{
		class NEAPI Image : public Asset<Image>
		{
		public:
			Image();
			Image(const Assets::ImageData& src_data, const Importers::ImageLoadingDesc& Desc);
			~Image();

			bool CreateTextureFromRawImage(const Assets::ImageData& src_data, const Importers::ImageLoadingDesc& Desc);

			ImageData mData;
			RefCntAutoPtr<ITextureView> mTextureView;
		};
	}
}