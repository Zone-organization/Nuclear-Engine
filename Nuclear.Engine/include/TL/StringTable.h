#pragma once
#include <unordered_set>
#include <TL/String.h>

namespace Nuclear
{
	namespace TL
	{
		class StringTable
		{
		public:

			std::unordered_set<TString> mTable;

		};
	}
}