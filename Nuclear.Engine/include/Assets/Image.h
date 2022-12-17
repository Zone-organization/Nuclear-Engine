#pragma once
#include <Assets/Common.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Importers/Common.h>

namespace Nuclear
{
	namespace Assets
	{
		class NEAPI Image : public Asset
		{
		public:
			Image();
			Image(const Assets::ImageData& src_data, const Importers::ImageLoadingDesc& Desc);
			~Image();

			bool CreateTextureFromRawImage(const Assets::ImageData& src_data, const Importers::ImageLoadingDesc& Desc);


	//	protected:
			//friend class Rendering::ShadowPass;

			ImageData mData;
			RefCntAutoPtr<ITextureView> mTextureView;
		protected:

			friend Serialization::Access;

			template<typename S>
			void serialize(S& s)
			{
				s.value1b(mType);
				s.value4b(mHashedName);
				s.text1b(mName, 256);
			}
		};
	}
}