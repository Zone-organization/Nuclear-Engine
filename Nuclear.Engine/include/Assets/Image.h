#pragma once
#include <Assets/IAsset.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>

namespace Nuclear
{
	namespace Graphics
	{
		class GraphicsEngine;
	}
	namespace Assets
	{			
		class NEAPI Image : public IAsset
		{
		public:
			Image();
			~Image();

			void SetTextureView(ITextureView* view);
			ITextureView* GetTextureView();

			const Uint32 GetWidth() const;
			const Uint32 GetHeight() const;
		protected:
			friend class Graphics::GraphicsEngine;

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