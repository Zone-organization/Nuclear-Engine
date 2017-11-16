#pragma once
#include <vector>

namespace NuclearEngine {
	enum class DataType;
}

namespace NuclearRenderer {

	class NRBInputLayout
	{
	public:
		struct ILElement
		{
			int index = 0;
			const char* name = "";
			NuclearEngine::DataType dataType;
			unsigned int stride = 0;
			unsigned int offset = 0;
		};

		void Push(const char* SemanticName, int SemanticIndex, NuclearEngine::DataType dataType, unsigned int stride, unsigned int offset)
		{
			ILElement Block;

			Block.index = SemanticIndex;
			Block.name = SemanticName;
			Block.dataType = dataType;
			Block.stride = stride;
			Block.offset = offset;

			Elements.push_back(Block);
		}

		std::vector<ILElement> GetBufferElement()
		{
			return Elements;			
		}

	protected:
		std::vector<ILElement> Elements;
	};
}