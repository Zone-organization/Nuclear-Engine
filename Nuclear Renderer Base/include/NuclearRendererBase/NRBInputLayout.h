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
		};

		void Push(const char* SemanticName, int SemanticIndex, NuclearEngine::DataType dataType)
		{
			ILElement Block;

			Block.index = SemanticIndex;
			Block.name = SemanticName;
			Block.dataType = dataType;
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