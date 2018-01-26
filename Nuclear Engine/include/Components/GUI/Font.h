#pragma once
#include <NE_Common.h>
#include <Math\Math.h>
#include <API\Texture.h>
#include <API\VertexBuffer.h>
#include <API\Color.h>
#include <map>
namespace NuclearEngine
{
	namespace Components 
	{
		namespace GUI
		{
			// Holds all state information relevant to a character as loaded using FreeType
			struct Character {
				API::Texture Texture;   // ID handle of the glyph texture
				Math::Vector2i Size;    // Size of glyph
				Math::Vector2i Bearing;  // Offset from baseline to left/top of glyph
				unsigned int Advance;    // Horizontal offset to advance to next glyph
			};

			class Font {
			public:
				Font();
				~Font();

				bool Create(std::string path, unsigned int size);

				static void InitializeTextRenderer(std::string VSPath, std::string PSPATH);
				void RenderText(std::string text, float x, float y, float scale, API::Color color);
			private:
				std::map<char, Character> Characters;
				API::VertexBuffer Font_VB;
			};
		}
	}
}