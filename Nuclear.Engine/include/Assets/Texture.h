#pragma once
#include <Assets/IAsset.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>

namespace Nuclear
{
	namespace Graphics
	{
		class GraphicsModule;
	}
	namespace Assets
	{			
		class NEAPI Texture : public IAsset
		{
		public:
			Texture();
			~Texture();

			void SetTextureView(Diligent::ITextureView* view);
			Diligent::ITextureView* GetTextureView();

			const Uint32 GetWidth() const;
			const Uint32 GetHeight() const;

		protected:
			friend class Graphics::GraphicsModule;
			friend Serialization::Access;

			Diligent::RefCntAutoPtr<Diligent::ITextureView> mTextureView;
			Uint32 mWidth, mHeight;
		};
	}
}