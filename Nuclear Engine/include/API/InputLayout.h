#pragma once
#include <NE_Common.h>
#include <API\API_Types.h>
#include <vector>

namespace NuclearEngine {
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

			void Push(const char* SemanticName, int SemanticIndex, DataType dataType);
			
			std::vector<ILElement> GetBufferElement();

		protected:
			std::vector<ILElement> Elements;
		};
	}
}	