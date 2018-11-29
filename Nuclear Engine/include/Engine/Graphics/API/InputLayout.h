#pragma once
#include <Base\NE_Common.h>
#include <Engine/Graphics/API/Format.h>
#include <vector>

namespace NuclearEngine {
	namespace Graphics
	{
		namespace API {
			struct ILElement
			{
				int index = 0;
				const char* name = "";
				Format format;
			};

			class NEAPI InputLayout
			{
			public:
				InputLayout();
				~InputLayout();

				void AppendAttribute(const char* SemanticName, int SemanticIndex, Format format);

				bool AutoAlignedOffset = true;
				std::vector<ILElement> GetBufferElement();

			protected:
				std::vector<ILElement> Elements;
			};
		}
	}
}	