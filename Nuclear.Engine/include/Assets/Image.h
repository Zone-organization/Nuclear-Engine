#pragma once
#include <Assets/IAsset.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Assets/ImportingDescs.h>

namespace Nuclear
{
	namespace Assets
	{
		class NEAPI Image : public IAsset
		{
		public:
			Image();
			Image(const Assets::ImageData& src_data, const ImageImportingDesc& Desc);
			~Image();

			//TODO: replace ImageImportingDesc with ImageCreationDesc.
			bool CreateTextureFromRawImage(const Assets::ImageData& src_data, const ImageImportingDesc& Desc);


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
				//s.value4b(mHashedName);
				s.text1b(mName, 256);
			}
		};
	}
}