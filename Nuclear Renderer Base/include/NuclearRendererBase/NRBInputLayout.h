#pragma once

#include <vector>

namespace NuclearRenderer {
	enum class DataType { Float, Float2, Float3, Float4 };

	class NRBInputLayout
	{
	public:
		struct ILElement
		{
			int index = 0;
			const char* name = "";
			DataType dataType;
			unsigned int stride = 0;
			unsigned int offset = 0;
		};

		void Push(int index, const char* name, DataType dataType, unsigned int stride, unsigned int offset)
		{
			ILElement Block;

			Block.index = index;
			Block.name = name;
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