#pragma once
#include <unordered_set>
#include <Types/String.h>

namespace Nuclear
{
	namespace Types
	{
		class StringTable
		{
		public:

			std::unordered_set<TString> mTable;

		};
	}
}