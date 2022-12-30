#pragma once
#include <Assets/IAsset.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Assets/ImportingDescs.h>

namespace Nuclear
{
	namespace Assets
	{
		struct ImageCreationDesc
		{
			ImageCreationDesc()
			{
				mData = Assets::ImageData();
				mImportingDesc = ImageImportingDesc();
			}
			ImageCreationDesc(const Assets::ImageData& data, const ImageImportingDesc& desc)
				: mData(data), mImportingDesc(desc)
			{
				
			}
			Assets::ImageData mData;
			ImageImportingDesc mImportingDesc;
		};

		class NEAPI Image : public IAsset
		{
		public:
			Image();
			~Image();

			bool Create(const ImageCreationDesc& Desc);

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