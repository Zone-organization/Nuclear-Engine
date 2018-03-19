#pragma once
#include <NE_Common.h>
#include <Graphics/API/Buffer_Types.h>
#include <vector>

namespace NuclearEngine {
	namespace Graphics
	{
		namespace API {
			struct ILElement
			{
				int index = 0;
				const char* name = "";
				DataType dataType;
			};

			class NEAPI InputLayout
			{
			public:
				InputLayout();
				~InputLayout();

				void AppendAttribute(const char* SemanticName, int SemanticIndex, DataType dataType);

				std::vector<ILElement> GetBufferElement();

			protected:
				std::vector<ILElement> Elements;
			};
		}
	}
}	