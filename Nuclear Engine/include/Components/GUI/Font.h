#pragma once
#include <NE_Common.h>

namespace NuclearEngine
{
	namespace Components
	{
		namespace GUI
		{
			class Font
			{
			public:
				Font();

				static void Load_TTF_Font(Font* font, std::string filepath, float size);
				static void Bind();

			};

		}
	}
}