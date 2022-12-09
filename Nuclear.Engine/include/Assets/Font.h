#pragma once
#include <Assets/Common.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Math/Math.h>
#include <unordered_map>

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
		struct FontCharacter {
			Uint32 CodePoint;               //Could be removed since it is stored in the map already
			Math::ivec2   Size;       // Size of glyph
			Math::ivec2   Bearing;    // Offset from baseline to left/top of glyph
			unsigned int Advance;    // Offset to advance to next glyph
		};

		class NEAPI Font : public Asset<Font>
		{
		public:
			Font();
			~Font();


			void Create(const FontCreationDesc& desc);


		//private:
			std::unordered_map<Uint32, FontCharacter> Characters;

			RefCntAutoPtr<ITextureView> mTextureView;    //Font atlas
		};


	}
}