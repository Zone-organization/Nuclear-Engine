#include <Engine\Graphics\API\InputLayout.h>

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

			void InputLayout::AppendAttribute(const char* SemanticName, int SemanticIndex, Format sformat)
			{
				ILElement Block;

				Block.index = SemanticIndex;
				Block.name = SemanticName;
				Block.format = sformat;
				Elements.push_back(Block);
			}

			std::vector<ILElement> InputLayout::GetBufferElement()
			{
				return Elements;
			}

		}
	}
}