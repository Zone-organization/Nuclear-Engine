#include "Graphics/API/InputLayout.h"
#include <Graphics\API\InputLayout.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			InputLayout::InputLayout()
			{
			}

			InputLayout::~InputLayout()
			{
			}

			void InputLayout::AppendAttribute(const char* SemanticName, int SemanticIndex, DataType dataType)
			{
				ILElement Block;

				Block.index = SemanticIndex;
				Block.name = SemanticName;
				Block.dataType = dataType;
				Elements.push_back(Block);
			}

			std::vector<ILElement> InputLayout::GetBufferElement()
			{
				return Elements;
			}

		}
	}
}