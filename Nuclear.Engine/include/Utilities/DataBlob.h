#pragma once
#include <NE_Common.h>
#include <vector>

namespace Nuclear
{
	namespace Utilities
	{
		class NEAPI DataBlob
		{
		public:
			DataBlob();
			DataBlob(const std::vector<Uint8>& buffer);
			~DataBlob();

			const std::vector<Uint8>& GetBuffer() const;
	//	private:
			std::vector<Uint8> mDataBuf;
		};

	}
}