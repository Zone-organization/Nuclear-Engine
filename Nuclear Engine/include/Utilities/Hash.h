#pragma once
#include <ThirdParty\crc32c.h>
#include <NE_PrivateCommon.h>
namespace NuclearEngine
{
	namespace Utilities
	{
		inline Uint32 NEAPI Hash(const char* data, size_t count)
		{
			return crc32c::Crc32c(data, count);
		}

		inline Uint32 NEAPI Hash(const std::string& string)
		{
			return crc32c::Crc32c(string);
		}
	}
}