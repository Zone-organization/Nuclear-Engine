#pragma once
#include <Engine/Assets/Common.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>

namespace Nuclear
{
	namespace Assets
	{
		struct FontCreationDesc
		{
			Uint32 AtlasWidth;
			Uint32 AtlasHeight;
			const void* Data;
		};

		class NEAPI Font : public Asset<Font>
		{
		public:
			Font();
			~Font();


			void Create(const FontCreationDesc& desc);


		//private:
			RefCntAutoPtr<ITextureView> mTextureView;    //Font atlas
		};


	}
}