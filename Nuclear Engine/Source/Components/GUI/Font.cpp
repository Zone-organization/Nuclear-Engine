#include "Components\GUI\Font.h"
#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
#include "stb_truetype.h"
#include <API\Texture.h>
namespace NuclearEngine
{
	namespace Components
	{
		namespace GUI
		{
			unsigned char ttf_buffer[1 << 20];
			unsigned char temp_bitmap[512 * 512];
			API::Texture ftex;
			stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
			void Font::Load_TTF_Font(Font * font, std::string filepath, float size)
			{
				fread(ttf_buffer, 1, 1 << 20, fopen(filepath.c_str(), "rb"));
				int i =	stbtt_BakeFontBitmap(ttf_buffer, 0, size, temp_bitmap, 512, 512, 32, 96, cdata); // no guarantee this fits!

				API::Texture_Desc TexDesc;
				TexDesc.Filter = API::TextureFilter::Linear2D;
				TexDesc.Wrap = API::TextureWrap::Repeat;
				TexDesc.Format = API::Format::R8;
				TexDesc.Type = API::TextureType::Texture2D;
				API::Texture_Data data;
				data.Width = data.Height = 512;
				data.Img_Data_Buf = temp_bitmap;
				API::Texture::Create(&ftex, data, TexDesc);
				// can free ttf_buffer at this poin
			}
			void Font::Bind()
			{
				ftex.PSBind(0);
			}
		}
	}
}