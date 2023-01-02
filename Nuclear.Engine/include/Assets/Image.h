#pragma once
#include <Assets/IAsset.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Assets/ImportingDescs.h>

namespace Nuclear
{
	namespace Assets
	{			
		struct ImageDesc
		{
			ImageDesc()
			{
				mTexDesc = Diligent::TextureDesc();
				mSubresources = std::vector<TextureSubResData>();
				mMips = std::vector< std::vector<Uint8> >();
			}
			Diligent::TextureDesc mTexDesc;
			std::vector<TextureSubResData> mSubresources;
			std::vector< std::vector<Uint8> > mMips;
		};

		class NEAPI Image : public IAsset
		{
		public:
			Image();
			~Image();

			bool Create(ImageDesc* desc);

			void CreateImageDesc(ImageDesc* desc, const Assets::ImageData& data);

			void SetTextureView(ITextureView* view);
			ITextureView* GetTextureView();

			const Uint32 GetWidth() const;
			const Uint32 GetHeight() const;
		protected:
			RefCntAutoPtr<ITextureView> mTextureView;
			Uint32 mWidth, mHeight;
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