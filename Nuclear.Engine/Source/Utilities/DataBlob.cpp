#include "Utilities\DataBlob.h"
namespace Nuclear
{
	namespace Utilities
	{
		DataBlob::DataBlob()
		{
		}
		DataBlob::DataBlob(const std::vector<Uint8>& buffer)
			: mDataBuf(buffer)
		{
		}
		DataBlob::~DataBlob()
		{
		}
		const std::vector<Uint8>& DataBlob::GetBuffer() const
		{
			return mDataBuf;
		}
	}
}